//
// Created by Angelo Carly on 09/01/2023.
//

#ifndef VKRT_VULKANSESSION_H
#define VKRT_VULKANSESSION_H

#include <memory>

namespace vks
{
    class VulkanSession
    {
        public:
            VulkanSession();
            ~VulkanSession();

        public:
            static VulkanSessionPtr GetInstance();
            vks::DevicePtr GetDevice();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_VULKANSESSION_H
