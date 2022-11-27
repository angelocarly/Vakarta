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
            Renderer();
            ~Renderer();

            void SetupVulkan();

        private:
            vks::PhysicalDevice mPhysicalDevice;
            vks::LogicalDevice mLogicalDevice;
    };
}


#endif //VKRT_RENDERER_H
