//
// Created by Angelo Carly on 03/12/2022.
//

#include "vks/SwapChain.h"

#include "vks/PhysicalDevice.h"
#include "vks/Device.h"
#include "vks/ForwardDecl.h"

#include <spdlog/spdlog.h>

// TODO: put all in Impl class
vks::SwapChain::SwapChain( const vks::DevicePtr inLogicalDevice, vk::SurfaceKHR inSurface )
:
    mDevice( inLogicalDevice ),
    mSurface( inSurface )
{
    spdlog::get( "vulkan" )->debug( "Initializing swapchain." );

    // Supported options for the created OS window
    auto physicalDevice = mDevice->GetPhysicalDevice();
    auto surfaceCapabilities = physicalDevice->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );
    auto surfaceFormats = physicalDevice->GetVkPhysicalDevice().getSurfaceFormatsKHR( mSurface );
    auto presentModes = physicalDevice->GetVkPhysicalDevice().getSurfacePresentModesKHR( mSurface );

    const uint32_t graphicsFamilyIndex = inLogicalDevice->GetPhysicalDevice()->FindQueueFamilyIndices().graphicsFamilyIndex.value();

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
        inSurface,
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
        vk::PresentModeKHR::eImmediate,
        true,
        nullptr
    );
    mSwapchain = mDevice->GetVkDevice().createSwapchainKHR( swapchainCreateInfoKhr );

    auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
    mImageAvailableForPresentingSemophore = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );
    mImageAvailableForRenderingSemophore = mDevice->GetVkDevice().createSemaphore( semaphoreCreateInfo );

    auto fenceCreateInfo = vk::FenceCreateInfo
    (
        vk::FenceCreateFlagBits::eSignaled // Mark the first image as signaled
    );
    mImageInFlightFence = mDevice->GetVkDevice().createFence( fenceCreateInfo );

    CreateSwapChainImages();
}

vks::SwapChain::~SwapChain()
{
    mDevice->GetVkDevice().waitForFences( mImageInFlightFence, true, UINT64_MAX );
    mDevice->GetVkDevice().waitIdle();

    for( int i = 0; i < mSwapChainImageViews.size(); i++ )
    {
        mDevice->GetVkDevice().destroy( mSwapChainImageViews[ i ] );
    }

    mDevice->GetVkDevice().destroy( mImageAvailableForRenderingSemophore );
    mDevice->GetVkDevice().destroy( mImageAvailableForPresentingSemophore );
    mDevice->GetVkDevice().destroy( mImageInFlightFence );
    mDevice->GetVkDevice().destroy( mSwapchain );
}

void
vks::SwapChain::CreateSwapChainImages()
{
    mSwapChainImages = mDevice->GetVkDevice().getSwapchainImagesKHR( mSwapchain );

    mSwapChainImageViews.resize( mSwapChainImages.size() );

    for (size_t i = 0; i < mSwapChainImages.size(); i++)
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

std::vector< vk::Image >
vks::SwapChain::GetSwapChainImages()
{
    return mSwapChainImages;
}

std::vector< vk::ImageView > vks::SwapChain::GetSwapChainImageViews()
{
    return mSwapChainImageViews;
}

vk::Format
vks::SwapChain::GetImageFormat()
{
    return vk::Format::eB8G8R8A8Unorm;
}

vk::SwapchainKHR vks::SwapChain::GetVkSwapchain()
{
    return mSwapchain;
}

int
vks::SwapChain::AcquireNextImage()
{
    mDevice->GetVkDevice().waitForFences( mImageInFlightFence, true, UINT64_MAX );
    mDevice->GetVkDevice().resetFences( mImageInFlightFence );

    uint32_t imageIndex = 0;
    mDevice->GetVkDevice().acquireNextImageKHR
    (
        mSwapchain,
        UINT64_MAX,
        nullptr,
        mImageInFlightFence,
        &imageIndex
    );

    return imageIndex;
}

void
vks::SwapChain::PresentSwapChain( uint32_t inImageIndex, vk::Semaphore & inWaitSemaphore )
{
    auto thePresentInfo = vk::PresentInfoKHR
    (
        inWaitSemaphore,
        mSwapchain,
        inImageIndex
    );
    auto result = mDevice->GetVkQueue().presentKHR( thePresentInfo );
    if( result != vk::Result::eSuccess )
    {
        throw std::runtime_error( "Error presenting swapchain" );
    }
}
