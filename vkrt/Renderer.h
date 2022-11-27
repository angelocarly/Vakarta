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

        private:
            vks::Instance & mInstance;
            vks::PhysicalDevice mPhysicalDevice;
            vks::LogicalDevice mLogicalDevice;

            vk::SurfaceKHR mSurface;

    };
}


#endif //VKRT_RENDERER_H
