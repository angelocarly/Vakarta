//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_PHYSICALDEVICE_H
#define VKRT_PHYSICALDEVICE_H

#include <vulkan/vulkan.hpp>
#include <optional>

#include "vks/Instance.h"
#include "ForwardDecl.h"

namespace vks
{
    struct QueueFamilyIndices
    {
        std::optional< uint32_t > graphicsFamilyIndex;

        bool IsComplete()
        {
            return graphicsFamilyIndex.has_value();
        }
    };

    class PhysicalDevice
    {
        public:
            explicit PhysicalDevice( vks::Instance & inInstance );
            ~PhysicalDevice();

            vk::PhysicalDevice GetVulkanPhysicalDevice();
            QueueFamilyIndices FindQueueFamilyIndices();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_PHYSICALDEVICE_H
