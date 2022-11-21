//
// Created by Angelo Carly on 21/11/2022.
//

#ifndef VKRT_VKSPHYSICALDEVICE_H
#define VKRT_VKSPHYSICALDEVICE_H

#include <vulkan/vulkan.hpp>

namespace vks
{
    class PhysicalDevice
    {
        public:
            PhysicalDevice();
            ~PhysicalDevice();

        private:
            vk::PhysicalDevice mPhysicalDevice;
    };
}

#endif //VKRT_VKSPHYSICALDEVICE_H
