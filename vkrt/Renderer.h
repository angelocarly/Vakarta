//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_RENDERER_H
#define VKRT_RENDERER_H

#include "vks/LogicalDevice.h"
#include "vks/PhysicalDevice.h"

namespace vkrt
{
    class Renderer
    {
        public:
            Renderer( vk::SurfaceKHR inSurface );
            ~Renderer();

        private:
            void InitializeQueue();

        private:
            vks::Instance & mInstance;
            vks::PhysicalDevice mPhysicalDevice;
            vks::LogicalDevice mLogicalDevice;
            vk::SurfaceKHR mSurface;

            vk::Queue mQueue;

    };
}


#endif //VKRT_RENDERER_H
