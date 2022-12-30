//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/Renderer.h"

#include "vks/Vertex.h"
#include "vkrt/Window.h"

#include <spdlog/spdlog.h>

#include <memory>

vkrt::Renderer::Renderer( vkrt::WindowPtr inWindow )
:
    mInstance( vks::Instance::GetInstance() ),
    mWindow( inWindow ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mDevice( std::make_shared< vks::Device >( mPhysicalDevice ) ),
    mSwapChain( mDevice, mWindow->GetVkSurface() )
{
    InitializeRenderPass();
    InitializeFrameBuffers();
    InitializeCommandBuffers();
    InitializeSynchronizationObject();
    InitializeRenderObject();
    mPipeline = std::make_unique<vks::Pipeline>( mDevice, mRenderPass );
}

vkrt::Renderer::~Renderer()
{
    mDevice->GetVkDevice().waitIdle();
    mDevice->GetVkDevice().destroy( mPresentSemaphore );
    mDevice->GetVkDevice().destroy( mRenderPass );

    for( int i = 0; i < mFrameBuffers.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mFrameBuffers[ i ] );
    }
}

// =====================================================================================================================

void
vkrt::Renderer::InitializeRenderPass()
{

    auto theDepthStencilAttachment = vk::AttachmentReference
    (
        1,
        vk::ImageLayout::eDepthAttachmentOptimal
    );

    vk::AttachmentReference const theColorAttachmentReference( 0, vk::ImageLayout::eColorAttachmentOptimal );

    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        &theColorAttachmentReference,
        nullptr,
        nullptr, //TODO: add depth attachment
        0,
        nullptr
    );

    // Why?
    std::array< vk::SubpassDependency, 1 > theSubPassDependencies;
    theSubPassDependencies[ 0 ].setSrcSubpass( VK_SUBPASS_EXTERNAL );
    theSubPassDependencies[ 0 ].setDstSubpass( 0 );
    theSubPassDependencies[ 0 ].setSrcStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    theSubPassDependencies[ 0 ].setSrcAccessMask( vk::AccessFlagBits::eNone );
    theSubPassDependencies[ 0 ].setDstStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput );
    theSubPassDependencies[ 0 ].setDstAccessMask( vk::AccessFlagBits::eColorAttachmentWrite );

    auto theColorAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        mSwapChain.GetImageFormat(),
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    );

    // TODO: Add depth attachment
    auto theDepthAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        mSwapChain.GetImageFormat(),
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eDepthStencilAttachmentOptimal
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

    mRenderPass = mDevice->GetVkDevice().createRenderPass( theRenderPassCreateInfo );

}

void
vkrt::Renderer::InitializeFrameBuffers()
{
    mFrameBuffers = mSwapChain.CreateFrameBuffers( mRenderPass );
}

void
vkrt::Renderer::InitializeCommandBuffers()
{
    mCommandBuffers.resize( mFrameBuffers.size() );
    for( int i = 0; i < mCommandBuffers.size(); ++i )
    {
        mCommandBuffers[ i ] = mDevice->GetVkDevice().allocateCommandBuffers
        (
            vk::CommandBufferAllocateInfo( mDevice->GetVkCommandPool(), vk::CommandBufferLevel::ePrimary, 1 )
        ).front();
    }
}

void
vkrt::Renderer::InitializeSynchronizationObject()
{
    mPresentSemaphore = mDevice->GetVkDevice().createSemaphore
    (
        vk::SemaphoreCreateInfo
        (
            vk::SemaphoreCreateFlags()
        )
    );
}

void
vkrt::Renderer::InitializeRenderObject()
{
    std::vector< vks::Vertex > theVertices =
    {
        vks::Vertex( glm::vec3( 100, 100, 0 ) ),
        vks::Vertex( glm::vec3( 100, 600, 0 ) ),
        vks::Vertex( glm::vec3( 600, 600, 0 ) ),
        vks::Vertex( glm::vec3( 600, 100, 0 ) )
    };
    std::vector< uint32_t > theIndices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    mMesh = std::make_unique< vks::Mesh >( mDevice, theVertices, theIndices );
}

// ---------------------------------------------------------------------------------------------------------------------

void vkrt::Renderer::Render()
{
    uint32_t theImageIndex = mSwapChain.RetrieveNextImage();

    auto theSurfaceCapabilities = mPhysicalDevice->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( mWindow->GetVkSurface() );

    auto theCommandBuffer = mCommandBuffers[ theImageIndex ];
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {
        auto theClearValue = vk::ClearValue
        (
            vk::ClearColorValue( std::array< float, 4 >( { 0, 0, 0, 0 } ) )
        );
        auto theRenderPassBeginInfo = vk::RenderPassBeginInfo
        (
            mRenderPass,
            mFrameBuffers[ theImageIndex ],
            vk::Rect2D( vk::Offset2D( 0, 0 ), theSurfaceCapabilities.currentExtent ),
            1,
            & theClearValue
        );
        theCommandBuffer.beginRenderPass( theRenderPassBeginInfo, vk::SubpassContents::eInline );
        {
            auto theViewport = vk::Viewport
            (
                0,
                0,
                theSurfaceCapabilities.currentExtent.width,
                theSurfaceCapabilities.currentExtent.height,
                0.0f,
                1.0f
            );
            theCommandBuffer.setViewport( 0, 1, & theViewport );
            const auto theScissors = vk::Rect2D( { 0, 0 }, theSurfaceCapabilities.currentExtent );
            theCommandBuffer.setScissor( 0, 1, & theScissors );

            mPipeline->UpdatePipelineUniforms( theSurfaceCapabilities.currentExtent.width, theSurfaceCapabilities.currentExtent.height );
            mPipeline->Bind( theCommandBuffer );
            mMesh->Draw( theCommandBuffer );
        }
        theCommandBuffer.endRenderPass();
    }
    theCommandBuffer.end();

    mSwapChain.SubmitCommandBuffer( theImageIndex, theCommandBuffer );
}
