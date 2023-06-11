//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/graphics/Display.h"

#include "vkrt/graphics/RenderEnvironment.h"
#include "vkrt/graphics/Layer.h"
#include "vkrt/graphics/Presenter.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Window.h"
#include "vks/render/Utils.h"
#include "vkrt/graphics/GuiLayer.h"
#include "vks/render/DescriptorLayoutBuilder.h"

#include <spdlog/spdlog.h>

#include <memory>

vkrt::Display::Display( vks::VulkanSessionPtr inSession, vks::WindowPtr inWindow )
:
    mWindow( inWindow ),
    mDevice( inSession->GetDevice() ),
    mSwapchain( std::make_shared< vks::Swapchain >( mDevice, mWindow->GetVkSurface() ) )
{
    InitializeCommandBuffers();
    InitializeRenderPass();
    InitializeFrameBuffers();
}

vkrt::Display::~Display()
{
    for( auto theFrameBuffer : mFrameBuffers )
    {
        mDevice->GetVkDevice().destroyFramebuffer( theFrameBuffer );
    }
    mDevice->GetVkDevice().destroyRenderPass( mRenderPass );
}

// =====================================================================================================================

/**
 * @brief Draw a single frame to the screen
 * @param inMesh
 */
void
vkrt::Display::Render()
{
    uint32_t theFrameIndex = mSwapchain->RetrieveNextImage();

    // Begin the current frame's draw commands
    auto theCommandBuffer = mCommandBuffers[ theFrameIndex ];
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {
        RenderEnvironment theRenderEnvironment =
        {
            theFrameIndex,
            vk::Rect2D( { 0, 0 }, { mSwapchain->GetExtent().width, mSwapchain->GetExtent().height } ),
            theCommandBuffer,
            mRenderPass
        };

        // Prepare the presenter
        if( mPresenter )
        {
            mPresenter->Prepare( theRenderEnvironment );
        }

        // Set viewport and scissor
        auto theViewport = vk::Viewport
        (
            0,
            0,
            mSwapchain->GetExtent().width,
            mSwapchain->GetExtent().height,
            0.0f,
            1.0f
        );
        theCommandBuffer.setViewport( 0, 1, & theViewport );
        const auto theScissors = vk::Rect2D( { 0, 0 }, mSwapchain->GetExtent() );
        theCommandBuffer.setScissor( 0, 1, & theScissors );

        // Draw the presenter to the frame
        theCommandBuffer.beginRenderPass( CreateRenderPassBeginInfo( theFrameIndex ), vk::SubpassContents::eInline );
        {
            if( mPresenter )
            {
                mPresenter->Draw( theRenderEnvironment );
            }
        }
        theCommandBuffer.endRenderPass();
    }
    theCommandBuffer.end();

    mSwapchain->SubmitCommandBuffer( theFrameIndex, theCommandBuffer );
}

void
vkrt::Display::SetPresenter( std::shared_ptr< Presenter > inPresenter )
{
    mPresenter = inPresenter;
}


void
vkrt::Display::InitializeCommandBuffers()
{
    auto theCommandBufferCount = mSwapchain->GetImageCount();
    mCommandBuffers.resize( theCommandBufferCount );
    for( int i = 0; i < theCommandBufferCount; i++ )
    {
        mCommandBuffers[ i ] = mDevice->GetVkDevice().allocateCommandBuffers
        (
            vk::CommandBufferAllocateInfo( mDevice->GetVkCommandPool(), vk::CommandBufferLevel::ePrimary, 1 )
        ).front();
    }
}

void
vkrt::Display::InitializeRenderPass()
{
    auto theColorAttachmentReference = vk::AttachmentReference
    (
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        & theColorAttachmentReference,
        nullptr,
        nullptr,
        0,
        nullptr
    );

    // The depth attachment is first accessed in the early fragment stage.
    std::array< vk::SubpassDependency, 1 > theSubPassDependencies;
    theSubPassDependencies[ 0 ].setSrcSubpass( VK_SUBPASS_EXTERNAL );
    theSubPassDependencies[ 0 ].setDstSubpass( 0 );
    theSubPassDependencies[ 0 ].setSrcStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
    theSubPassDependencies[ 0 ].setSrcAccessMask( vk::AccessFlagBits::eNone );
    theSubPassDependencies[ 0 ].setDstStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
    theSubPassDependencies[ 0 ].setDstAccessMask( vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite );

    auto theColorAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        mSwapchain->GetImageFormat(),
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    );

    std::vector< vk::AttachmentDescription > theAttachmentDescriptions =
    {
        theColorAttachmentDescription
    };
    auto theRenderPassCreateInfo = vk::RenderPassCreateInfo
    (
        vk::RenderPassCreateFlags(),
        theAttachmentDescriptions.size(),
        theAttachmentDescriptions.data(),
        1,
        &theSubpassDescription,
        theSubPassDependencies.size(),
        theSubPassDependencies.data()
    );

    mRenderPass = mDevice->GetVkDevice().createRenderPass
    (
        theRenderPassCreateInfo
    );

    mClearValue = vk::ClearValue( vk::ClearColorValue( std::array< float, 4 >{ 0.0f, 0.0f, 0.0f, 1.0f } ) );
}

void
vkrt::Display::InitializeFrameBuffers()
{
    auto theSwapchainImageViews = mSwapchain->GetSwapchainImageViews();
    mFrameBuffers.resize( theSwapchainImageViews.size() );

    for ( size_t i = 0; i < mFrameBuffers.size(); i++)
    {
        std::array< vk::ImageView, 1 > theAttachments =
        {
            theSwapchainImageViews[ i ]
        };
        auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            theAttachments.size(),
            theAttachments.data(),
            mSwapchain->GetExtent().width,
            mSwapchain->GetExtent().height,
            1
        );
        mFrameBuffers[i] = mDevice->GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
    }
}

vk::RenderPassBeginInfo
vkrt::Display::CreateRenderPassBeginInfo( std::size_t inFrameIndex )
{
    return vk::RenderPassBeginInfo
    (
        mRenderPass,
        mFrameBuffers[ inFrameIndex ],
        vk::Rect2D( vk::Offset2D( 0, 0 ), mSwapchain->GetExtent() ),
        mClearValue
    );
}
