//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_LOGICALDEVICE_H
#define VKRT_LOGICALDEVICE_H

#include <vulkan/vulkan.hpp>

#include "Instance.h"
#include "PhysicalDevice.h"

namespace vks
{
    class LogicalDevice
    {
        public:
            explicit LogicalDevice( vks::PhysicalDevice & inDevice );
            ~LogicalDevice();

        private:
            class Impl;

            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_LOGICALDEVICE_H
