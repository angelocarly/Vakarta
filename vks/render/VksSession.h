//
// Created by Angelo Carly on 09/01/2023.
//

#ifndef VKRT_VKSSESSION_H
#define VKRT_VKSSESSION_H

#include <memory>

namespace vks
{
    class VksSession
    {
        public:
            VksSession();
            ~VksSession();

        public:
            static VulkanSessionPtr GetInstance();
            vks::DevicePtr GetDevice();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_VKSSESSION_H
