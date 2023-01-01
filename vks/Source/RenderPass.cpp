//
// Created by Angelo Carly on 01/01/2023.
//

#include "vks/RenderPass.h"

#include "vks/ForwardDecl.h"
#include "vks/Device.h"
#include "vks/Swapchain.h"

#include <vulkan/vulkan.hpp>

class vks::RenderPass::Impl
{
    public:
        Impl( vks::SwapchainPtr inSwapchain );
        ~Impl();

    public:
        vks::DevicePtr mDevice;
        vks::SwapchainPtr mSwapchain;
    public:
        vk::RenderPass mRenderPass;
        std::vector< vk::Framebuffer > mFrameBuffers;
        vk::ClearValue mClearValue;

    private:
        void InitializeRenderPass();
        void InitializeFrameBuffers();
};

vks::RenderPass::Impl::Impl( vks::SwapchainPtr inSwapchain )
:
    mSwapchain( inSwapchain ),
    mDevice( inSwapchain->GetDevice() )
{
    InitializeRenderPass();
    InitializeFrameBuffers();
}

vks::RenderPass::Impl::~Impl()
{
    mDevice->GetVkDevice().destroy( mRenderPass );

    for( int i = 0; i < mFrameBuffers.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mFrameBuffers[ i ] );
    }
}

void
vks::RenderPass::Impl::InitializeRenderPass()
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
        mSwapchain->GetImageFormat(),
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
        mSwapchain->GetImageFormat(),
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

    mClearValue = vk::ClearValue
    (
        vk::ClearColorValue( std::array< float, 4 >( { 0, 0, 0, 0 } ) )
    );
}

void
vks::RenderPass::Impl::InitializeFrameBuffers()
{
    auto theSwapchainImageViews = mSwapchain->GetSwapchainImageViews();
    mFrameBuffers.resize( theSwapchainImageViews.size() );

    for ( size_t i = 0; i < mFrameBuffers.size(); i++)
    {
        auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            1,
            &theSwapchainImageViews[ i ],
            mSwapchain->GetExtent().width,
            mSwapchain->GetExtent().height,
            1
        );
        mFrameBuffers[i] = mDevice->GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
    }
}


// ---------------------------------------------------------------------------------------------------------------------

vks::RenderPass::RenderPass( vks::SwapchainPtr inSwapchain )
:
    mImpl( new vks::RenderPass::Impl( inSwapchain ) )
{

}

vks::RenderPass::~RenderPass()
{

}

vk::RenderPass
vks::RenderPass::GetVkRenderPass()
{
    return mImpl->mRenderPass;
}

vk::RenderPassBeginInfo
vks::RenderPass::GetVkBeginInfo( int inImageIndex )
{
    auto theRenderPassBeginInfo = vk::RenderPassBeginInfo
    (
        mImpl->mRenderPass,
        mImpl->mFrameBuffers[ inImageIndex ],
        vk::Rect2D( vk::Offset2D( 0, 0 ), mImpl->mSwapchain->GetExtent() ),
        1,
        & mImpl->mClearValue
    );
    return theRenderPassBeginInfo;
}
