//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_LOGICALDEVICE_H
#define VKRT_LOGICALDEVICE_H

#include <vulkan/vulkan.hpp>

#include "Instance.h"
#include "ForwardDecl.h"

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

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_LOGICALDEVICE_H
