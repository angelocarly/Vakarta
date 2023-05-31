/**
 * OffScreenRenderPass.cpp
 *
 * @file	OffScreenRenderPass.cpp
 * @author	Angelo Carly
 * @date	09/05/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "vks/render/OffScreenRenderPass.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/Device.h"
#include "vks/render/Swapchain.h"

#include <vulkan/vulkan.hpp>

#include <array>

class vks::OffScreenRenderPass::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, std::size_t inConcurrentCount, vk::Format inFormat, std::size_t inWidth, std::size_t inHeight );
        ~Impl();

    public:
        vks::DevicePtr mDevice;

    public:
        vk::RenderPass mRenderPass;
        std::array< vk::ClearValue, 2 > mClearValues;

        vk::Format mFormat;
        std::size_t mWidth;
        std::size_t mHeight;
        std::size_t mConcurrentCount;

        std::vector< vks::Image > mImages;
        std::vector< vk::ImageView > mImageViews;
        std::vector< vk::Framebuffer > mFrameBuffers;

        vks::Image mDepthImage;
        vk::ImageView mDepthImageView;

    private:
        vk::Format GetDepthFormat();

    private:
        void InitializeImages();
        void InitializeRenderPass();
        void InitializeFrameBuffers();
        void InitializeDepthImage();
};

vks::OffScreenRenderPass::Impl::Impl( vks::DevicePtr inDevice, std::size_t inConcurrentCount, vk::Format inFormat, std::size_t inWidth, std::size_t inHeight )
:
    mDevice( inDevice ),
    mFormat( inFormat ),
    mWidth( inWidth ),
    mHeight( inHeight ),
    mConcurrentCount( inConcurrentCount )
{
    InitializeImages();
    InitializeDepthImage();
    InitializeRenderPass();
    InitializeFrameBuffers();
}

vks::OffScreenRenderPass::Impl::~Impl()
{
    mDevice->GetVkDevice().destroy( mRenderPass );

    for( int i = 0; i < mFrameBuffers.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mFrameBuffers[ i ] );
    }

    for( int i = 0; i < mImageViews.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mImageViews[ i ] );
    }

    for( int i = 0; i < mImages.size(); i++ )
    {
        mDevice->DestroyImage( mImages[ i ] );
    }

    mDevice->DestroyImage( mDepthImage );
    mDevice->GetVkDevice().destroy( mDepthImageView );
}

void
vks::OffScreenRenderPass::Impl::InitializeImages()
{
    for( size_t i = 0; i < mConcurrentCount; i++ )
    {
        auto theImageCreateInfo = vk::ImageCreateInfo
        (
            vk::ImageCreateFlags(),
            vk::ImageType::e2D,
            mFormat,
            vk::Extent3D( mWidth, mHeight, 1 ),
            1,
            1,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment,
            vk::SharingMode::eExclusive,
            0,
            0,
            vk::ImageLayout::eUndefined
        );
        auto theAllocationInfo = vma::AllocationCreateInfo
        (
            vma::AllocationCreateFlagBits::eDedicatedMemory,
            vma::MemoryUsage::eAuto
        );
        mImages.push_back( mDevice->CreateImage( theImageCreateInfo, theAllocationInfo ) );
    }

    mImageViews.resize( mConcurrentCount );

    for ( size_t i = 0; i < mImageViews.size(); i++)
    {
        auto imageViewCreateInfo = vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            mImages[ i ].GetVkImage(),
            vk::ImageViewType::e2D,
            mFormat,
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
        );

        mImageViews[ i ] = mDevice->GetVkDevice().createImageView( imageViewCreateInfo );
    }
}

void
vks::OffScreenRenderPass::Impl::InitializeRenderPass()
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
        mFormat,
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
vks::OffScreenRenderPass::Impl::InitializeFrameBuffers()
{
    mFrameBuffers.resize( mImageViews.size() );

    for ( size_t i = 0; i < mFrameBuffers.size(); i++)
    {
        std::array< vk::ImageView, 2 > theAttachments =
        {
            mImageViews[ i ],
            mDepthImageView
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
        mFrameBuffers[i] = mDevice->GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
    }
}

void
vks::OffScreenRenderPass::Impl::InitializeDepthImage()
{
    // Image
    std::uint32_t theQueueFamilyIndex = mDevice->GetPhysicalDevice()->FindQueueFamilyIndices().graphicsFamilyIndex.value();
    auto theImageCreateInfo = vk::ImageCreateInfo
    (
        vk::ImageCreateFlags(),
        vk::ImageType::e2D,
        GetDepthFormat(),
        vk::Extent3D( mWidth, mHeight, 1 ),
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

vk::Format vks::OffScreenRenderPass::Impl::GetDepthFormat()
{
    return vk::Format::eD32Sfloat;
}


// ---------------------------------------------------------------------------------------------------------------------

vks::OffScreenRenderPass::OffScreenRenderPass( vks::DevicePtr inDevice, std::size_t inConcurrentCount, vk::Format inFormat, std::size_t inWidth, std::size_t inHeight )
:
    mImpl( new vks::OffScreenRenderPass::Impl( inDevice, inConcurrentCount, inFormat, inWidth, inHeight ) )
{

}

vks::OffScreenRenderPass::~OffScreenRenderPass()
{

}

vk::RenderPass
vks::OffScreenRenderPass::GetVkRenderPass()
{
    return mImpl->mRenderPass;
}

vk::RenderPassBeginInfo
vks::OffScreenRenderPass::GetVkBeginInfo( int inImageIndex )
{
    auto theRenderPassBeginInfo = vk::RenderPassBeginInfo
    (
        mImpl->mRenderPass,
        mImpl->mFrameBuffers[ inImageIndex ],
        vk::Rect2D( vk::Offset2D( 0, 0 ), vk::Extent2D( mImpl->mWidth, mImpl->mHeight ) ),
        mImpl->mClearValues.size(),
        mImpl->mClearValues.data()
    );
    return theRenderPassBeginInfo;
}
