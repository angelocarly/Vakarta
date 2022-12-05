//
// Created by Angelo Carly on 03/12/2022.
//

#include "vks/SwapChain.h"

#include "vks/PhysicalDevice.h"
#include "vks/LogicalDevice.h"
#include "vks/ForwardDecl.h"

#include <spdlog/spdlog.h>

vks::SwapChain::SwapChain( const vks::LogicalDevicePtr inLogicalDevice, vk::SurfaceKHR inSurface )
:
    mLogicalDevice( inLogicalDevice ),
    mSurface( inSurface )
{
    spdlog::get( "vulkan" )->debug( "Initializing swapchain." );

    // Supported options for the created OS window
    auto physicalDevice = mLogicalDevice->GetPhysicalDevice();
    auto surfaceCapabilities = physicalDevice->GetVulkanPhysicalDevice().getSurfaceCapabilitiesKHR( mSurface );
    auto surfaceFormats = physicalDevice->GetVulkanPhysicalDevice().getSurfaceFormatsKHR( mSurface );
    auto presentModes = physicalDevice->GetVulkanPhysicalDevice().getSurfacePresentModesKHR( mSurface );

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
        vk::PresentModeKHR::eFifo,
        true,
        nullptr
    );
    mSwapchain = mLogicalDevice->GetVulkanDevice().createSwapchainKHR( swapchainCreateInfoKhr );

    auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
    mImageAvailableForPresentingSemophore = mLogicalDevice->GetVulkanDevice().createSemaphore( semaphoreCreateInfo );
    mImageAvailableForRenderingSemophore = mLogicalDevice->GetVulkanDevice().createSemaphore( semaphoreCreateInfo );

    auto fenceCreateInfo = vk::FenceCreateInfo
    (
        vk::FenceCreateFlagBits::eSignaled // Mark the first image as signaled
    );
    mImageInFlightFence = mLogicalDevice->GetVulkanDevice().createFence( fenceCreateInfo );
}

vks::SwapChain::~SwapChain()
{
    mLogicalDevice->GetVulkanDevice().destroy( mImageAvailableForRenderingSemophore );
    mLogicalDevice->GetVulkanDevice().destroy( mImageAvailableForPresentingSemophore );
    mLogicalDevice->GetVulkanDevice().destroy( mImageInFlightFence );
    mLogicalDevice->GetVulkanDevice().destroy( mSwapchain );
}

std::vector< vk::Image >
vks::SwapChain::GetSwapChainImages()
{
    return mLogicalDevice->GetVulkanDevice().getSwapchainImagesKHR( mSwapchain );
}

vk::Format
vks::SwapChain::GetImageFormat()
{
    return vk::Format::eB8G8R8A8Unorm;
}

void
vks::SwapChain::AcquireNextImage()
{
//    mLogicalDevice->GetVulkanDevice().waitForFences( mImageInFlightFence, true, UINT64_MAX );
//    mLogicalDevice->GetVulkanDevice().resetFences( mImageInFlightFence );

//    uint32_t imageIndex = 0;
//    mLogicalDevice->GetVulkanDevice().acquireNextImageKHR
//    (
//        mSwapchain,
//        UINT64_MAX,
//        nullptr,
//        mImageInFlightFence,
//        &imageIndex
//    );

//    mLogicalDevice->GetVulkanDevice().getSwapchainImagesKHR( mSwapchain );
}