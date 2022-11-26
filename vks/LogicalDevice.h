//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_LOGICALDEVICE_H
#define VKRT_LOGICALDEVICE_H

#include <vulkan/vulkan.hpp>

namespace vks
{
    class LogicalDevice
    {
        public:
            LogicalDevice();
            ~LogicalDevice();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_LOGICALDEVICE_H
