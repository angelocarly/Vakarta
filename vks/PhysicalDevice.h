//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_PHYSICALDEVICE_H
#define VKRT_PHYSICALDEVICE_H

#include <vulkan/vulkan.hpp>

#include "vks/Instance.h"

namespace vks
{
    struct QueueFamilyIndices
    {
        uint32_t graphicsFamilyIndex;
    };

    class PhysicalDevice
    {
        public:
            explicit PhysicalDevice( vks::Instance & inInstance );
            ~PhysicalDevice();

            vk::PhysicalDevice GetVulkanPhysicalDevice();
            QueueFamilyIndices GetQueueFamilyIndices();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_PHYSICALDEVICE_H
