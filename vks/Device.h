//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_DEVICE_H
#define VKRT_DEVICE_H

#include <vulkan/vulkan.hpp>

#include "Instance.h"
#include "ForwardDecl.h"
#include "Buffer.h"
#include "Image.h"

namespace vks
{
    class Device
    {
        public:
            explicit Device( vks::PhysicalDevicePtr inDevice );
            ~Device();

            vks::PhysicalDevicePtr GetPhysicalDevice();
            vk::Device GetVkDevice();
            vk::CommandPool GetVkCommandPool();
            vk::Queue GetVkQueue();

            vk::CommandBuffer BeginSingleTimeCommands();
            void EndSingleTimeCommands( vk::CommandBuffer & inCommandBuffer );
            vks::Buffer CreateBuffer( vk::BufferCreateInfo inBufferCreateInfo, vma::AllocationCreateInfo inAllocationCreateInfo );
            void DestroyBuffer( vks::Buffer inBuffer );
            vks::Image CreateImage( vk::ImageCreateInfo inImageCreateInfo, vma::AllocationCreateInfo inAllocationCreateInfo );
            void DestroyImage( vks::Image inImage );
            void * MapMemory( vks::Buffer inBuffer );
            void UnmapMemory( vks::Buffer inBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_DEVICE_H
