//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_RENDERER_H
#define VKRT_RENDERER_H

#include "vks/LogicalDevice.h"
#include "vks/PhysicalDevice.h"
#include "vks/SwapChain.h"

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
            vks::PhysicalDevicePtr mPhysicalDevice;
            vks::LogicalDevicePtr mLogicalDevice;

            // Display
            vk::SurfaceKHR mSurface;
            vks::SwapChain mSwapChain;

            // Commands
            vk::Queue mQueue;

            void InitializeSwapChain();
    };
}


#endif //VKRT_RENDERER_H
