//
// Created by Angelo Carly on 03/12/2022.
//

#include "vks/SwapChain.h"

#include "vks/PhysicalDevice.h"
#include "vks/Device.h"
#include "vks/ForwardDecl.h"

#include <spdlog/spdlog.h>

// ---------------------------------------------------------------------------------------------------------------------

class vks::SwapChain::Impl
{
    public:
        Impl( const vks::DevicePtr inDevice, vk::SurfaceKHR inSurface );
        ~Impl();

        vk::Format GetImageFormat();

    public:
        vks::DevicePtr mDevice;
        vk::SurfaceKHR mSurface;
        vk::SwapchainKHR mSwapchain;
    public:
        std::vector< vk::Image > mSwapChainImages;
        std::vector< vk::ImageView > mSwapChainImageViews;

        int mCurrentFrame = 0;
        std::vector< vk::Fence > mCommandBufferExecutedFence;
        std::vector< vk::Semaphore > mImageAcquiredSemaphores;
        std::vector< vk::Semaphore > mBufferPresentedSemaphores;

    private:
        void InitializeSwapChain();
        void InitializeSwapChainImages();
        void InitializeSynchronizationObjects();

        vk::ColorSpaceKHR GetColorSpace();
};

vks::SwapChain::Impl::Impl( const vks::DevicePtr inDevice, vk::SurfaceKHR inSurface )
:
    mDevice( inDevice ),
    mSurface( inSurface )
{
    InitializeSwapChain();
    InitializeSwapChainImages();
    InitializeSynchronizationObjects();
}

vks::SwapChain::Impl::~Impl()
{
    mDevice->GetVkDevice().waitForFences( mCommandBufferExecutedFence, true, UINT64_MAX );
    mDevice->GetVkDevice().waitIdle();

    for( int i = 0; i < mSwapChainImageViews.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mSwapChainImageViews[ i ] );
    }

    for( auto theFence : mCommandBufferExecutedFence )
    {
        mDevice->GetVkDevice().destroy( theFence );
    }
    for( auto theSemaphore : mImageAcquiredSemaphores )
    {
        mDevice->GetVkDevice().destroy( theSemaphore );
    }
    for( auto theSemaphore : mBufferPresentedSemaphores )
    {
        mDevice->GetVkDevice().destroy( theSemaphore );
    }

    mDevice->GetVkDevice().destroy( mSwapchain );
}

vk::Format
vks::SwapChain::Impl::GetImageFormat()
{
    return vk::Format::eB8G8R8A8Srgb;
}

vk::ColorSpaceKHR
vks::SwapChain::Impl::GetColorSpace()
{
    return vk::ColorSpaceKHR::eSrgbNonlinear;
}

void
vks::SwapChain::Impl::InitializeSwapChain()
{
    spdlog::get( "vulkan" )->debug( "Initializing swapchain." );

    // Supported options for the created OS window
    auto physicalDevice = mDevice->GetPhysicalDevice();
    auto surfaceCapabilities = physicalDevice->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );
    auto surfaceFormats = physicalDevice->GetVkPhysicalDevice().getSurfaceFormatsKHR( mSurface );
    auto presentModes = physicalDevice->GetVkPhysicalDevice().getSurfacePresentModesKHR( mSurface );

    const uint32_t graphicsFamilyIndex = mDevice->GetPhysicalDevice()->FindQueueFamilyIndices().graphicsFamilyIndex.value();

    auto theFormat = GetImageFormat();
    auto theColorSpace = GetColorSpace();

    // Verify whether the surface supports the colorformat/colorspace
    bool hasFormat = false;
    for( auto surfaceFormat : surfaceFormats )
    {
        if( surfaceFormat.format == theFormat && surfaceFormat.colorSpace == theColorSpace )
        {
            hasFormat = true;
            break;
        }
    }
    if( !hasFormat )
    {
        spdlog::get( "vulkan" )->error
        (
            "The surface doesn't support the requested format ({}) with colorspace ({})",
            to_string( theFormat ),
            to_string( theColorSpace )
        );
        std::exit( 1 );
    }

    vk::SwapchainCreateInfoKHR swapchainCreateInfoKhr
    (
        vk::SwapchainCreateFlagsKHR(),
        mSurface,
        3,
        GetImageFormat(),
        GetColorSpace(),
        surfaceCapabilities.currentExtent, // Initialize swapchain images with the current extent
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        1,
        &graphicsFamilyIndex,
        vk::SurfaceTransformFlagBitsKHR::eIdentity,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        vk::PresentModeKHR::eFifo,
        true,
        nullptr
    );
    mSwapchain = mDevice->GetVkDevice().createSwapchainKHR( swapchainCreateInfoKhr );

}

void
vks::SwapChain::Impl::InitializeSwapChainImages()
{
    mSwapChainImages = mDevice->GetVkDevice().getSwapchainImagesKHR( mSwapchain );
    mSwapChainImageViews.resize( mSwapChainImages.size() );

    for (size_t i = 0; i < mSwapChainImageViews.size(); i++)
    {
        auto imageViewCreateInfo = vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            mSwapChainImages[ i ],
            vk::ImageViewType::e2D,
            GetImageFormat(),
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

        mSwapChainImageViews[ i ] = mDevice->GetVkDevice().createImageView( imageViewCreateInfo );
    }
}

void
vks::SwapChain::Impl::InitializeSynchronizationObjects()
{
    mCommandBufferExecutedFence.resize( mSwapChainImages.size() );
    for( int i = 0; i < mCommandBufferExecutedFence.size(); ++i )
    {
        auto fenceCreateInfo = vk::FenceCreateInfo
        (
            vk::FenceCreateFlagBits::eSignaled // Mark the first image as signaled
        );
        mCommandBufferExecutedFence[ i ] = mDevice->GetVkDevice().createFence( fenceCreateInfo );
    }

    mImageAcquiredSemaphores.resize( mSwapChainImages.size() );
    for( int i = 0; i < mImageAcquiredSemaphores.size(); ++i )
    {
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
        mImageAcquiredSemaphores[ i ] = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );
    }

    mBufferPresentedSemaphores.resize( mSwapChainImages.size() );
    for( int i = 0; i < mBufferPresentedSemaphores.size(); ++i )
    {
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() );
        mBufferPresentedSemaphores[ i ] = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

vks::SwapChain::SwapChain( const vks::DevicePtr inLogicalDevice, vk::SurfaceKHR inSurface )
:
    mImpl( new Impl( inLogicalDevice, inSurface ) )
{
}

vks::SwapChain::~SwapChain()
{
}

vk::SwapchainKHR
vks::SwapChain::GetVkSwapchain()
{
    return mImpl->mSwapchain;
}

vk::Format vks::SwapChain::GetImageFormat()
{
    return mImpl->GetImageFormat();
}

/**
 * Wait for the next presentable image index.
 * @return
 */
int
vks::SwapChain::RetrieveNextImage()
{
    mImpl->mCurrentFrame = ( mImpl->mCurrentFrame + 1 ) % mImpl->mSwapChainImages.size();

    while( vk::Result::eTimeout == mImpl->mDevice->GetVkDevice().waitForFences( mImpl->mCommandBufferExecutedFence[ mImpl->mCurrentFrame ], VK_TRUE, UINT64_MAX ));

    // Block until a new image is acquired
    uint32_t imageIndex = 0;
    mImpl->mDevice->GetVkDevice().acquireNextImageKHR
    (
        mImpl->mSwapchain,
        UINT64_MAX,
        mImpl->mImageAcquiredSemaphores[ mImpl->mCurrentFrame ],
        nullptr,
        &imageIndex
    );

    return imageIndex;
}

void
vks::SwapChain::SubmitCommandBuffer( uint32_t inImageIndex, vk::CommandBuffer inCommandBuffer )
{
    mImpl->mDevice->GetVkDevice().resetFences( mImpl->mCommandBufferExecutedFence[ mImpl->mCurrentFrame ] );
    mImpl->mDevice->GetVkQueue().submit
    (
        vk::SubmitInfo
        (
            1,
            &mImpl->mImageAcquiredSemaphores[ mImpl->mCurrentFrame ],
            nullptr,
            1,
            &inCommandBuffer,
            1,
            &mImpl->mBufferPresentedSemaphores[ inImageIndex ]
        ),
        mImpl->mCommandBufferExecutedFence[ mImpl->mCurrentFrame ]
    );

    // Display the swapchain image
    PresentImage( inImageIndex, mImpl->mBufferPresentedSemaphores[ inImageIndex ] );
}

/**
 * Present a swapchain image
 * @param inImageIndex the image to render
 * @param inWaitSemaphore semaphore to trigger before submission
 */
void
vks::SwapChain::PresentImage( uint32_t inImageIndex, vk::Semaphore & inWaitSemaphore )
{
    // Submit an image for presentation
    auto thePresentInfo = vk::PresentInfoKHR
    (
        inWaitSemaphore,
        mImpl->mSwapchain,
        inImageIndex
    );

    auto result = mImpl->mDevice->GetVkQueue().presentKHR( thePresentInfo );
    if( result != vk::Result::eSuccess )
    {
        throw std::runtime_error( "Error presenting swapchain" );
    }
}

std::vector< vk::Framebuffer >
vks::SwapChain::CreateFrameBuffers( vk::RenderPass inRenderPass )
{
    std::vector< vk::Framebuffer > theFrameBuffers;
    theFrameBuffers.resize( mImpl->mSwapChainImageViews.size() );

    for ( size_t i = 0; i < theFrameBuffers.size(); i++)
    {
        auto theSurfaceCapabilities = mImpl->mDevice->GetPhysicalDevice()->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( mImpl->mSurface );

        auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            inRenderPass,
            1,
            & mImpl->mSwapChainImageViews[ i ],
            theSurfaceCapabilities.currentExtent.width,
            theSurfaceCapabilities.currentExtent.height,
            1
        );
        theFrameBuffers[i] = mImpl->mDevice->GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
    }

    return theFrameBuffers;
}

