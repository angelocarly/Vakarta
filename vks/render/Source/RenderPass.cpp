//
// Created by Angelo Carly on 01/01/2023.
//

#include "vks/render/RenderPass.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/Device.h"
#include "vks/render/Swapchain.h"

#include <vulkan/vulkan.hpp>

#include <array>

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
        std::array< vk::ClearValue, 2 > mClearValues;

        std::vector< vk::Framebuffer > mFrameBuffers;
        vks::Image mDepthImage;
        vk::ImageView mDepthImageView;

    private:
        vk::Format GetDepthFormat();

    private:
        void InitializeRenderPass();
        void InitializeFrameBuffers();
        void InitializeDepthImage();
};

vks::RenderPass::Impl::Impl( vks::SwapchainPtr inSwapchain )
:
    mSwapchain( inSwapchain ),
    mDevice( inSwapchain->GetDevice() )
{
    InitializeDepthImage();
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

    mDevice->DestroyImage( mDepthImage );
    mDevice->GetVkDevice().destroy( mDepthImageView );
}

void
vks::RenderPass::Impl::InitializeRenderPass()
{
    auto theColorAttachmentReference = vk::AttachmentReference
    (
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );
    auto theDepthStencilAttachmentReference = vk::AttachmentReference
    (
        1,
        vk::ImageLayout::eDepthStencilAttachmentOptimal
    );
    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        &theColorAttachmentReference,
        nullptr,
        &theDepthStencilAttachmentReference,
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

    auto theDepthAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        GetDepthFormat(),
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
        theColorAttachmentDescription,
        theDepthAttachmentDescription
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

    // Clear colors
    mClearValues[ 0 ] = vk::ClearValue
    (
        vk::ClearColorValue( std::array< float, 4 >( { 0, 0, 0, 0 } ) )
    );
    mClearValues[ 1 ] = vk::ClearValue
    (
        vk::ClearDepthStencilValue( 1.0f, 0 )
    );
}

void
vks::RenderPass::Impl::InitializeFrameBuffers()
{
    auto theSwapchainImageViews = mSwapchain->GetSwapchainImageViews();
    mFrameBuffers.resize( theSwapchainImageViews.size() );

    for ( size_t i = 0; i < mFrameBuffers.size(); i++)
    {
        std::array< vk::ImageView, 2 > theAttachments =
        {
            theSwapchainImageViews[ i ],
            mDepthImageView
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

void
vks::RenderPass::Impl::InitializeDepthImage()
{
    // Image
    std::uint32_t theQueueFamilyIndex = mDevice->GetPhysicalDevice()->FindQueueFamilyIndices().graphicsFamilyIndex.value();
    auto theImageCreateInfo = vk::ImageCreateInfo
    (
        vk::ImageCreateFlags(),
        vk::ImageType::e2D,
        GetDepthFormat(),
        vk::Extent3D( mSwapchain->GetExtent().width, mSwapchain->GetExtent().height, 1 ),
        1,
        1,
        vk::SampleCountFlagBits::e1,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eDepthStencilAttachment,
        vk::SharingMode::eExclusive,
        1,
        &theQueueFamilyIndex,
        vk::ImageLayout::eUndefined
    );
    auto theAllocationInfo = vma::AllocationCreateInfo
    (
        vma::AllocationCreateFlagBits::eDedicatedMemory,
        vma::MemoryUsage::eAuto
    );
    mDepthImage = mDevice->CreateImage
    (
        theImageCreateInfo,
        theAllocationInfo
    );

    // Image View
    auto theImageViewCreateInfo = vk::ImageViewCreateInfo
    (
        vk::ImageViewCreateFlags(),
        mDepthImage.GetVkImage(),
        vk::ImageViewType::e2D,
        GetDepthFormat(),
        vk::ComponentMapping
        (
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity
        ),
        vk::ImageSubresourceRange
        (
            vk::ImageAspectFlagBits::eDepth,
            0,
            1,
            0,
            1
        )
    );
    mDepthImageView = mDevice->GetVkDevice().createImageView
    (
        theImageViewCreateInfo
    );
}

vk::Format vks::RenderPass::Impl::GetDepthFormat()
{
    return vk::Format::eD32Sfloat;
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
        mImpl->mClearValues.size(),
        mImpl->mClearValues.data()
    );
    return theRenderPassBeginInfo;
}
