//
// Created by Angelo Carly on 21/11/2022.
//

#include "vks/render/Device.h"

#include "vks/core/Buffer.h"
#include "vks/core/Image.h"
#include "vks/render/PhysicalDevice.h"
#include "vks/render/Utils.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.hpp>
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
        vma::Allocator mAllocator;

    public:
        void InitializeLogicalDevice();
        void InitializeCommandPool();
        void InitializeMemoryAllocator();
};

// ---------------------------------------------------------------------------------------------------------------------


vks::Device::Impl::Impl( const vks::PhysicalDevicePtr inPhysicalDevice )
:
    mPhysicalDevice( inPhysicalDevice )
{
    InitializeLogicalDevice();
    InitializeCommandPool();
    InitializeMemoryAllocator();
}

vks::Device::Impl::~Impl()
{
    mDevice.waitIdle();
    mAllocator.destroy();
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

    auto theRequiredExtensions = mPhysicalDevice->GetRequiredExtensions();
    vk::DeviceCreateInfo theDeviceCreateInfo
    (
        vk::DeviceCreateFlags(),
        std::uint32_t( theDeviceQueueCreateInfos.size() ),
        theDeviceQueueCreateInfos.data(),
        0,
        nullptr,
        std::uint32_t( theRequiredExtensions.size() ),
        theRequiredExtensions.data(),
        & theDeviceFeatures
    );

    for( const char * theExtensionName : mPhysicalDevice->GetRequiredExtensions() )
    {
        spdlog::get( "vulkan" )->debug( "- {}", theExtensionName );
    }

    mDevice = mPhysicalDevice->GetVkPhysicalDevice().createDevice( theDeviceCreateInfo );
    mQueue = mDevice.getQueue( mPhysicalDevice->FindQueueFamilyIndices().graphicsFamilyIndex.value(), 0 );
}

void
vks::Device::Impl::InitializeCommandPool()
{
    mCommandPool = mDevice.createCommandPool
    (
        vk::CommandPoolCreateInfo
        (
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            mPhysicalDevice->FindQueueFamilyIndices().graphicsFamilyIndex.value()
        )
    );
}

void
vks::Device::Impl::InitializeMemoryAllocator()
{
    vma::AllocatorCreateInfo theAllocatorInfo
    (
        vma::AllocatorCreateFlags(),
        mPhysicalDevice->GetVkPhysicalDevice(),
        mDevice
    );
    theAllocatorInfo.instance = vks::Instance::GetInstance().GetVkInstance();
    mAllocator = vma::createAllocator( theAllocatorInfo );

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

vks::PhysicalDevicePtr
vks::Device::GetPhysicalDevice()
{
    return mImpl->mPhysicalDevice;
}

vk::Device
vks::Device::GetVkDevice()
{
    return mImpl->mDevice;
}

vk::Queue
vks::Device::GetVkQueue()
{
    return mImpl->mQueue;
}

vma::Allocator
vks::Device::GetVmaAllocator()
{
    return mImpl->mAllocator;
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

vk::CommandPool
vks::Device::GetVkCommandPool()
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

vks::Buffer
vks::Device::CreateBuffer( vk::BufferCreateInfo inBufferCreateInfo, vma::AllocationCreateInfo inAllocationCreateInfo )
{
    auto theBuffer = mImpl->mAllocator.createBuffer( inBufferCreateInfo, inAllocationCreateInfo );
    return { theBuffer.first, theBuffer.second, inBufferCreateInfo.size };
}

void
vks::Device::DestroyBuffer( vks::Buffer inBuffer )
{
    mImpl->mAllocator.destroyBuffer( inBuffer.GetVkBuffer(), inBuffer.GetVmaAllocation() );
}

vks::Image
vks::Device::CreateImage( vk::ImageCreateInfo inImageCreateInfo, vma::AllocationCreateInfo inAllocationCreateInfo )
{
    auto theImageResult = mImpl->mAllocator.createImage( inImageCreateInfo, inAllocationCreateInfo );
    return { theImageResult.first, theImageResult.second };
}

void
vks::Device::DestroyImage( vks::Image inImage )
{
    mImpl->mAllocator.destroyImage( inImage.GetVkImage(), inImage.GetAllocation() );
}

void
vks::Device::ImageMemoryBarrier
(
    vk::CommandBuffer inCommandBuffer,
    vks::Image inImage,
    vk::Flags< vk::AccessFlagBits > inSrcAccessMask,
    vk::Flags< vk::AccessFlagBits > inDstAccessMask,
    vk::Flags< vk::PipelineStageFlagBits > inSrcStageMask,
    vk::Flags< vk::PipelineStageFlagBits > inDstStageMask,
    vk::ImageLayout inOldLayout,
    vk::ImageLayout inNewLayout,
    vk::DependencyFlags inDependencyFlags
)
{
    vk::ImageSubresourceRange theImageSubResourceRange
    (
        vk::ImageAspectFlagBits::eColor,
        0,
        1,
        0,
        1
    );
    vk::ImageMemoryBarrier theImageMemoryBarrier
    (
        inSrcAccessMask,
        inDstAccessMask,
        inOldLayout,
        inNewLayout,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        inImage.GetVkImage(),
        theImageSubResourceRange,
        nullptr
    );
    inCommandBuffer.pipelineBarrier
    (
        inSrcStageMask,
        inDstStageMask,
        inDependencyFlags,
        nullptr,
        nullptr,
        theImageMemoryBarrier
    );
}

void *
vks::Device::MapMemory( vks::Buffer inBuffer )
{
    return mImpl->mAllocator.mapMemory( inBuffer.GetVmaAllocation() );
}

void
vks::Device::UnmapMemory( vks::Buffer inBuffer )
{
    mImpl->mAllocator.unmapMemory( inBuffer.GetVmaAllocation() );
}