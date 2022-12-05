//
// Created by Angelo Carly on 21/11/2022.
//

#include "vks/LogicalDevice.h"

#include "vks/PhysicalDevice.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include <optional>

// =====================================================================================================================

class vks::LogicalDevice::Impl
{
    public:

    public:
        explicit Impl( const vks::PhysicalDevicePtr inPhysicalDevice );
        ~Impl();

    public:
        vks::PhysicalDevicePtr mPhysicalDevice;

        vk::Device mDevice;
        vk::Queue mQueue;

    public:
        void InitializeLogicalDevice();
};

// ---------------------------------------------------------------------------------------------------------------------

vks::LogicalDevice::Impl::Impl( const vks::PhysicalDevicePtr inPhysicalDevice )
:
    mPhysicalDevice( inPhysicalDevice )
{
    InitializeLogicalDevice();
}

vks::LogicalDevice::Impl::~Impl()
{

}

void vks::LogicalDevice::Impl::InitializeLogicalDevice()
{
    spdlog::get( "vulkan" )->debug( "Initializing logical device." );

    vk::PhysicalDeviceFeatures theDeviceFeatures = vk::PhysicalDeviceFeatures();

    std::array< vk::DeviceQueueCreateInfo, 1 > theDeviceQueueCreateInfos;
    float theQueuePriority = 1.0f;
    theDeviceQueueCreateInfos[ 0 ] = vk::DeviceQueueCreateInfo
    (
        vk::DeviceQueueCreateFlags(),
        mPhysicalDevice->FindQueueFamilyIndices().graphicsFamilyIndex.value(),
        1,
        & theQueuePriority
    );

    vk::DeviceCreateInfo theDeviceCreateInfo
    (
        vk::DeviceCreateFlags(),
        theDeviceQueueCreateInfos.size(),
        theDeviceQueueCreateInfos.data(),
        0,
        nullptr,
        kEnabledExtensions.size(),
        kEnabledExtensions.data(),
        & theDeviceFeatures
    );

    for( const char * theExtensionName : kEnabledExtensions )
    {
        spdlog::get( "vulkan" )->debug( "- {}", theExtensionName );
    }

    mDevice = mPhysicalDevice->GetVulkanPhysicalDevice().createDevice( theDeviceCreateInfo );
    mQueue = mDevice.getQueue( mPhysicalDevice->FindQueueFamilyIndices().graphicsFamilyIndex.value(), 0 );
}

// =====================================================================================================================

vks::LogicalDevice::LogicalDevice( const vks::PhysicalDevicePtr inPhysicalDevice )
:
    mImpl( new Impl( inPhysicalDevice ) )
{
}

vks::LogicalDevice::~LogicalDevice()
{
    mImpl->mDevice.waitIdle();
    mImpl->mDevice.destroy();
}

// =====================================================================================================================

vks::PhysicalDevicePtr vks::LogicalDevice::GetPhysicalDevice()
{
    return mImpl->mPhysicalDevice;
}

vk::Device vks::LogicalDevice::GetVulkanDevice()
{
    return mImpl->mDevice;
}

vk::Queue vks::LogicalDevice::GetQueue()
{
    return mImpl->mQueue;
}
