//
// Created by Angelo Carly on 21/11/2022.
//


#include <spdlog/spdlog.h>
#include "vks/PhysicalDevice.h"

class vks::PhysicalDevice::Impl
{
    public:
        Impl( vks::Instance & inInstance );

        ~Impl();

    public:
        void InitializeVulkanDevice();

    public:
        vks::Instance & mInstance;

        vk::PhysicalDevice mPhysicalDevice;
};

vks::PhysicalDevice::Impl::Impl( vks::Instance & inInstance )
:
mInstance( inInstance )
{
    InitializeVulkanDevice();
}

vks::PhysicalDevice::Impl::~Impl()
{
}

/**
 * Pick the most applicable Vulkan device
 */
void vks::PhysicalDevice::Impl::InitializeVulkanDevice()
{
    spdlog::get( "vulkan" )->debug( "Initializing physical device.." );

    std::vector< vk::PhysicalDevice > thePotentialDevices = mInstance.GetVulkanInstance().enumeratePhysicalDevices();
    for( vk::PhysicalDevice thePotentialDevice: thePotentialDevices )
    {
        auto theDeviceProperties = thePotentialDevice.getProperties();
        spdlog::get( "vulkan" )->debug( "- ID: {}", theDeviceProperties.deviceID );
        spdlog::get( "vulkan" )->debug( "- Name: {}", theDeviceProperties.deviceName );
        spdlog::get( "vulkan" )->debug( "- Device type: {}", to_string( theDeviceProperties.deviceType ) );
        spdlog::get( "vulkan" )->debug( "- API version: {}", theDeviceProperties.apiVersion );
        spdlog::get( "vulkan" )->debug( "- Driver version: {}", theDeviceProperties.driverVersion );

        std::vector< vk::QueueFamilyProperties > theQueueFamilyProperties = thePotentialDevice.getQueueFamilyProperties();
        for( vk::QueueFamilyProperties theQueueFamilyProperty: theQueueFamilyProperties )
        {
            if( theQueueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics )
            {
                mPhysicalDevice = thePotentialDevice;
                return;
            }
        }
    }

    spdlog::get( "vulkan" )->error( "Could not find a suitable physical device." );
    std::exit( 1 );
}

// =====================================================================================================================

vks::PhysicalDevice::PhysicalDevice( vks::Instance & inInstance )
:
mImpl( new Impl( inInstance ) )
{

}

vks::PhysicalDevice::~PhysicalDevice()
{

}

vk::PhysicalDevice vks::PhysicalDevice::GetVulkanPhysicalDevice()
{
    return mImpl->mPhysicalDevice;
}

vks::QueueFamilyIndices vks::PhysicalDevice::GetQueueFamilyIndices()
{
    QueueFamilyIndices theQueueFamilyIndices{};

    int index = 0;
    for( vk::QueueFamilyProperties theQueueFamilyProperty : mImpl->mPhysicalDevice.getQueueFamilyProperties() )
    {
        if( theQueueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics )
        {
            theQueueFamilyIndices.graphicsFamilyIndex = index;
        }

        index++;
    }

    return theQueueFamilyIndices;
}
