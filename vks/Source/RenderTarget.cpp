#include "vks/RenderTarget.h"

#include "vks/Buffer.h"
#include "vks/Image.h"
#include "vks/ForwardDecl.h"
#include "vks/Device.h"

#include <vulkan/vulkan.hpp>

class vks::RenderTarget::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vk::RenderPass inRenderPass, size_t inWidth, size_t inHeight );
        ~Impl();

        void CreateImageAndView();
        void CreateFrameBuffer();

    public:
        vks::DevicePtr mDevice;
        vk::RenderPass mRenderPass;

        size_t mWidth;
        size_t mHeight;

        vk::Framebuffer mFrameBuffer;
        vks::Image mFrameBufferImage;
        vk::ImageView mFrameBufferImageView;
};

vks::RenderTarget::Impl::Impl( vks::DevicePtr inDevice, vk::RenderPass inRenderPass, size_t inWidth, size_t inHeight )
:
    mDevice( inDevice ),
    mRenderPass( inRenderPass ),
    mWidth( inWidth ),
    mHeight( inHeight )
{
    // Create image
    vk::ImageCreateInfo theImageCreateInfo
    (
        vk::ImageCreateFlags(),
        vk::ImageType::e2D,
        vk::Format::eR8G8B8A8Unorm,
        vk::Extent3D( mWidth, mHeight, 1 ),
        1,
        1,
        vk::SampleCountFlagBits::e1,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferSrc,
        vk::SharingMode::eExclusive,
        {},
        vk::ImageLayout::eUndefined
    );
    vma::AllocationCreateInfo theImageAllocationCreateInfo;
    theImageAllocationCreateInfo.usage = vma::MemoryUsage::eAuto;
    theImageAllocationCreateInfo.flags = vma::AllocationCreateFlagBits::eDedicatedMemory;
    mFrameBufferImage = mDevice->CreateImage( theImageCreateInfo, theImageAllocationCreateInfo );

    // Create Image View
    vk::ImageViewCreateInfo const theImageViewCreateInfo
    (
        vk::ImageViewCreateFlags(),
        mFrameBufferImage.GetVkImage(),
        vk::ImageViewType::e2D,
        vk::Format::eR8G8B8A8Unorm,
        vk::ComponentMapping(),
        vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)
    );
    mFrameBufferImageView = mDevice->GetVkDevice().createImageView( theImageViewCreateInfo );

    std::array< vk::ImageView, 1 > theAttachments;
    theAttachments[0] = mFrameBufferImageView;

    vk::FramebufferCreateInfo const theFramebufferCreateInfo
    (
        vk::FramebufferCreateFlags(),
        mRenderPass,
        theAttachments,
        mWidth,
        mHeight,
        1
    );

    mFrameBuffer = mDevice->GetVkDevice().createFramebuffer( theFramebufferCreateInfo );
}

vks::RenderTarget::Impl::~Impl()
{}

// =====================================================================================================================

vks::RenderTarget::RenderTarget( vks::DevicePtr inDevice, vk::RenderPass inRenderPass, size_t inWidth, size_t inHeight )
:
    mImpl( new Impl( inDevice, inRenderPass, inWidth, inHeight ) )
{

}

vks::RenderTarget::~RenderTarget()
{
    mImpl->mDevice->GetVkDevice().destroyFramebuffer( mImpl->mFrameBuffer );
    mImpl->mDevice->GetVkDevice().destroyImageView( mImpl->mFrameBufferImageView );
    mImpl->mDevice->DestroyImage( mImpl->mFrameBufferImage );
}

size_t
vks::RenderTarget::GetWidth()
{
    return mImpl->mWidth;
}

size_t
vks::RenderTarget::GetHeight()
{
    return mImpl->mHeight;
}

vk::Framebuffer
vks::RenderTarget::GetFrameBuffer()
{
    return mImpl->mFrameBuffer;
}

vks::Image
vks::RenderTarget::GetFrameBufferImage()
{
    return mImpl->mFrameBufferImage;
}

vk::ImageView
vks::RenderTarget::GetFrameBufferImageView()
{
    return mImpl->mFrameBufferImageView;
}
