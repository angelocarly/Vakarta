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
        vk::Semaphore mImageAvailableForRenderingSemaphore;
        vk::Semaphore mImageAvailableForPresentingSemaphore;
        vk::Fence mImageInFlightFence;
        std::vector< vk::Image > mSwapChainImages;
        std::vector< vk::ImageView > mSwapChainImageViews;

    private:
        void InitializeSwapChain();
        void InitializeSwapChainImages();
};

vks::SwapChain::Impl::Impl( const vks::DevicePtr inDevice, vk::SurfaceKHR inSurface )
:
    mDevice( inDevice ),
    mSurface( inSurface )
{
    InitializeSwapChain();
    InitializeSwapChainImages();
}

vks::SwapChain::Impl::~Impl()
{
    mDevice->GetVkDevice().waitForFences( mImageInFlightFence, true, UINT64_MAX );
    mDevice->GetVkDevice().waitIdle();

    for( int i = 0; i < mSwapChainImageViews.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mSwapChainImageViews[ i ] );
    }

    mDevice->GetVkDevice().destroy( mImageAvailableForRenderingSemaphore );
    mDevice->GetVkDevice().destroy( mImageAvailableForPresentingSemaphore );
    mDevice->GetVkDevice().destroy( mImageInFlightFence );
    mDevice->GetVkDevice().destroy( mSwapchain );
}

vk::Format
vks::SwapChain::Impl::GetImageFormat()
{
    return vk::Format::eB8G8R8A8Unorm;
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

    auto theFormat = vk::Format::eB8G8R8A8Unorm;
    auto theColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

    // Verify whether the surface support the colorspace
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
        vk::ColorSpaceKHR::eSrgbNonlinear,
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

    auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
    mImageAvailableForPresentingSemaphore = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );
    mImageAvailableForRenderingSemaphore = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );

    auto fenceCreateInfo = vk::FenceCreateInfo
    (
        vk::FenceCreateFlagBits::eSignaled // Mark the first image as signaled
    );
    mImageInFlightFence = mDevice->GetVkDevice().createFence( fenceCreateInfo );
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
            vk::Format::eB8G8R8A8Unorm,
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
    mImpl->mDevice->GetVkDevice().resetFences( mImpl->mImageInFlightFence );

    // Block until a new image is acquired
    uint32_t imageIndex = 0;
    mImpl->mDevice->GetVkDevice().acquireNextImageKHR
    (
        mImpl->mSwapchain,
        UINT64_MAX,
        nullptr,
        mImpl->mImageInFlightFence,
        &imageIndex
    );

    // Wait until the image is no longer being read from by the window
    mImpl->mDevice->GetVkDevice().waitForFences( mImpl->mImageInFlightFence, true, UINT64_MAX );

    return imageIndex;
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

