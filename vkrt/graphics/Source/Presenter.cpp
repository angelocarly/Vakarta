//
// Created by magnias on 6/17/23.
//

#include "vkrt/graphics/Presenter.h"

#include "vks/render/Device.h"

vkrt::Presenter::Presenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight )
:
    mDevice( inDevice ),
    mWidth( inWidth ),
    mHeight( inHeight )
{
    InitializeImage();
    InitializeRenderPass();
    InitializeFrameBuffer( mImageView, mWidth, mHeight );
}

vkrt::Presenter::~Presenter()
{
    mDevice->GetVkDevice().destroy( mFrameBuffer );
    mDevice->GetVkDevice().destroy( mRenderPass );
    mDevice->GetVkDevice().destroy( mImageView );
    mDevice->GetVkDevice().destroy( mSampler );
    mDevice->DestroyImage( mImage );
}
// ============================================= Initialization ========================================================

void
vkrt::Presenter::InitializeImage()
{
    // Create image
    mImage = mDevice->CreateImage
    (
        mImageFormat,
        mWidth,
        mHeight,
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
        vma::AllocationCreateFlagBits::eDedicatedMemory
    );

    auto theCommandBuffer = mDevice->BeginSingleTimeCommands();
    {
        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            mImage,
            vk::AccessFlagBits::eShaderRead,
            vk::AccessFlagBits::eColorAttachmentWrite,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::DependencyFlagBits::eByRegion
        );
    }
    mDevice->EndSingleTimeCommands( theCommandBuffer );

    mImageView = mDevice->GetVkDevice().createImageView
    (
        vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            mImage.GetVkImage(),
            vk::ImageViewType::e2D,
            mImageFormat,
            vk::ComponentMapping
            (
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity
            ),
            vk::ImageSubresourceRange
            (
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        )
    );

    mSampler = mDevice->GetVkDevice().createSampler
    (
        vk::SamplerCreateInfo
        (
            vk::SamplerCreateFlags(),
            vk::Filter::eLinear,
            vk::Filter::eLinear,
            vk::SamplerMipmapMode::eLinear,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            0.0f,
            VK_FALSE,
            16.0f,
            VK_FALSE,
            vk::CompareOp::eAlways,
            0.0f,
            0.0f,
            vk::BorderColor::eIntOpaqueBlack,
            VK_FALSE
        )
    );
}

void
vkrt::Presenter::InitializeRenderPass()
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
        mImageFormat,
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
}

void
vkrt::Presenter::InitializeFrameBuffer( vk::ImageView inImageView, std::size_t inWidth, std::size_t inHeight )
{
    std::array< vk::ImageView, 1 > theAttachments =
    {
        mImageView
    };
    auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
    (
        vk::FramebufferCreateFlags(),
        mRenderPass,
        theAttachments.size(),
        theAttachments.data(),
        mWidth,
        mHeight,
        1
    );
    mFrameBuffer = mDevice->GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
}

void
vkrt::Presenter::BeginPresenterRenderPass( vk::CommandBuffer const & inCommandBuffer )
{
    // Transition the image to a color attachment
    mDevice->ImageMemoryBarrier
    (
        inCommandBuffer,
        mImage,
        vk::AccessFlagBits::eNone,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::ImageLayout::eShaderReadOnlyOptimal,
        vk::ImageLayout::eColorAttachmentOptimal,
        vk::DependencyFlagBits::eByRegion
    );

    // Set viewport and scissor
    auto theViewport = vk::Viewport
    (
        0,
        0,
        mWidth,
        mHeight,
        0.0f,
        1.0f
    );
    inCommandBuffer.setViewport( 0, 1, & theViewport );
    const auto theScissors = vk::Rect2D( { 0, 0 }, { mWidth, mHeight } );
    inCommandBuffer.setScissor( 0, 1, & theScissors );

    auto theClearValue = vk::ClearValue( vk::ClearColorValue( std::array< float, 4 >{ 0.0f, 0.0f, 0.0f, 0.0f } ) );
    auto theBeginInfo = vk::RenderPassBeginInfo
    (
        mRenderPass,
        mFrameBuffer,
        vk::Rect2D
        (
            vk::Offset2D( 0, 0 ),
            vk::Extent2D( mWidth, mHeight )
        ),
        1,
        & theClearValue
    );
    inCommandBuffer.beginRenderPass( theBeginInfo, vk::SubpassContents::eInline );
}

void
vkrt::Presenter::EndPresenterRenderPass( const vk::CommandBuffer & inCommandBuffer )
{
    inCommandBuffer.endRenderPass();

    // Transition the image to shader read so that it can be sampled
    mDevice->ImageMemoryBarrier
    (
        inCommandBuffer,
        mImage,
        vk::AccessFlagBits::eShaderWrite,
        vk::AccessFlagBits::eNone,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::PipelineStageFlagBits::eBottomOfPipe,
        vk::ImageLayout::ePresentSrcKHR,
        vk::ImageLayout::eShaderReadOnlyOptimal,
        vk::DependencyFlagBits::eByRegion
    );
}
