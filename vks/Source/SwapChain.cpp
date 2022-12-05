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
        vk::Format::eB8G8R8A8Unorm,
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
}

vks::SwapChain::~SwapChain()
{
    mLogicalDevice->GetVulkanDevice().destroy( mSwapchain );
}
