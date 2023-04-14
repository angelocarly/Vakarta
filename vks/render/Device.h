//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_DEVICE_H
#define VKRT_DEVICE_H

#include "vks/core/Buffer.h"
#include "vks/core/Image.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/Instance.h"

#include <vulkan/vulkan.hpp>

namespace vks
{
    class Device
    {
        public:
            explicit Device( vks::PhysicalDevicePtr inDevice );
            ~Device();

        public:
            vks::PhysicalDevicePtr GetPhysicalDevice();
            vk::Device GetVkDevice();
            vk::CommandPool GetVkCommandPool();
            vk::Queue GetVkQueue();
            vma::Allocator GetVmaAllocator();

        public:
            vk::CommandBuffer BeginSingleTimeCommands();
            void EndSingleTimeCommands( vk::CommandBuffer & inCommandBuffer );
            vks::Buffer CreateBuffer( vk::BufferCreateInfo inBufferCreateInfo, vma::AllocationCreateInfo inAllocationCreateInfo );
            void DestroyBuffer( vks::Buffer inBuffer );
            vks::Image CreateImage( vk::ImageCreateInfo inImageCreateInfo, vma::AllocationCreateInfo inAllocationCreateInfo );
            void DestroyImage( vks::Image inImage );
            void ImageMemoryBarrier
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
            );
            void * MapMemory( vks::Buffer inBuffer );
            void UnmapMemory( vks::Buffer inBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_DEVICE_H
