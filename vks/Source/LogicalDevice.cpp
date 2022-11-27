//
// Created by Angelo Carly on 21/11/2022.
//

#include <spdlog/spdlog.h>
#include "vks/LogicalDevice.h"

// =====================================================================================================================

class vks::LogicalDevice::Impl
{
    public:
        const std::vector< const char * > kEnabledExtensions = {

        };

    public:
        explicit Impl( vks::PhysicalDevice & inPhysicalDevice );
        ~Impl();

    public:
        vks::PhysicalDevice & mPhysicalDevice;

        vk::Device mDevice;

    public:
        void InitializeLogicalDevice();
};

// ---------------------------------------------------------------------------------------------------------------------

vks::LogicalDevice::Impl::Impl( vks::PhysicalDevice & inPhysicalDevice )
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
    spdlog::get( "vulkan" )->debug( "Initializing logical device.." );

    vk::PhysicalDeviceFeatures theDeviceFeatures = vk::PhysicalDeviceFeatures();

    std::array< vk::DeviceQueueCreateInfo, 1 > theDeviceQueueCreateInfos;
    float theQueuePriority = 1.0f;
    theDeviceQueueCreateInfos[ 0 ] = vk::DeviceQueueCreateInfo
    (
        vk::DeviceQueueCreateFlags(),
        mPhysicalDevice.GetQueueFamilyIndices().graphicsFamilyIndex,
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

    mDevice = mPhysicalDevice.GetVulkanPhysicalDevice().createDevice
    (
        theDeviceCreateInfo
    );
}

// =====================================================================================================================

vks::LogicalDevice::LogicalDevice( vks::PhysicalDevice & inPhysicalDevice )
:
    mImpl( new Impl( inPhysicalDevice ) )
{
}

vks::LogicalDevice::~LogicalDevice()
{}