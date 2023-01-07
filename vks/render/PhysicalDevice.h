//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_PHYSICALDEVICE_H
#define VKRT_PHYSICALDEVICE_H

#include "vks/render/ForwardDecl.h"
#include "vks/render/Instance.h"

#include <vulkan/vulkan.hpp>
#include <optional>

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

        public:
            vk::PhysicalDevice GetVkPhysicalDevice();
            QueueFamilyIndices FindQueueFamilyIndices();
            static std::vector< const char * > GetRequiredExtensions();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_PHYSICALDEVICE_H
