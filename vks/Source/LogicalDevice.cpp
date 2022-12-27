//
// Created by Angelo Carly on 21/11/2022.
//

#include "vks/LogicalDevice.h"

#include "vks/PhysicalDevice.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include <optional>

// =====================================================================================================================

class vks::Device::Impl
{
    public:

    public:
        explicit Impl( const vks::PhysicalDevicePtr inPhysicalDevice );
        ~Impl();

    public:
        vks::PhysicalDevicePtr mPhysicalDevice;

        vk::Device mDevice;
        vk::Queue mQueue;
        vk::CommandPool mCommandPool;

    public:
        void InitializeLogicalDevice();
        void InitializeCommandPool();
};

// ---------------------------------------------------------------------------------------------------------------------

vks::Device::Impl::Impl( const vks::PhysicalDevicePtr inPhysicalDevice )
:
    mPhysicalDevice( inPhysicalDevice )
{
    InitializeLogicalDevice();
    InitializeCommandPool();
}

vks::Device::Impl::~Impl()
{
    mDevice.waitIdle();
    mDevice.destroyCommandPool( mCommandPool );
    mDevice.destroy();
}

void
vks::Device::Impl::InitializeLogicalDevice()
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

void
vks::Device::Impl::InitializeCommandPool()
{
    mCommandPool = mDevice.createCommandPool
    (
        vk::CommandPoolCreateInfo
        (
            vk::CommandPoolCreateFlags(),
            mPhysicalDevice->FindQueueFamilyIndices().graphicsFamilyIndex.value()
        )
    );
}

// =====================================================================================================================

vks::Device::Device( const vks::PhysicalDevicePtr inPhysicalDevice )
:
    mImpl( new Impl( inPhysicalDevice ) )
{
}

vks::Device::~Device()
{
}

// =====================================================================================================================

vks::PhysicalDevicePtr vks::Device::GetPhysicalDevice()
{
    return mImpl->mPhysicalDevice;
}

vk::Device vks::Device::GetVkDevice()
{
    return mImpl->mDevice;
}

vk::Queue vks::Device::GetVkQueue()
{
    return mImpl->mQueue;
}

vk::CommandBuffer
vks::Device::BeginSingleTimeCommands()
{
    auto theCommandBuffer =  mImpl->mDevice.allocateCommandBuffers
    (
        vk::CommandBufferAllocateInfo( mImpl->mCommandPool, vk::CommandBufferLevel::ePrimary, 1 )
    ).front();
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    return theCommandBuffer;
}

vk::CommandPool vks::Device::GetVkCommandPool()
{
    return mImpl->mCommandPool;
}

void
vks::Device::EndSingleTimeCommands( vk::CommandBuffer & inCommandBuffer )
{
    inCommandBuffer.end();

    // Submit command buffer and wait until completion
    vk::Fence theFence = mImpl->mDevice.createFence( vk::FenceCreateInfo() );
    mImpl->mQueue.submit( vk::SubmitInfo( 0, nullptr, nullptr, 1, &inCommandBuffer ), theFence );
    while( vk::Result::eTimeout == mImpl->mDevice.waitForFences( theFence, VK_TRUE, UINT64_MAX ));
    mImpl->mDevice.destroyFence( theFence );

    mImpl->mDevice.freeCommandBuffers( mImpl->mCommandPool, inCommandBuffer );
}

