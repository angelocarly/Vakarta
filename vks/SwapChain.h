//
// Created by Angelo Carly on 03/12/2022.
//

#ifndef VKRT_SWAPCHAIN_H
#define VKRT_SWAPCHAIN_H

#include "PhysicalDevice.h"
#include "ForwardDecl.h"

#include <vulkan/vulkan.hpp>

namespace vks
{
    class SwapChain
    {
        public:
            SwapChain( vks::DevicePtr inLogicalDevice, vk::SurfaceKHR inSurface );
            ~SwapChain();

            int RetrieveNextImage();
            vk::SwapchainKHR GetVkSwapchain();
            void PresentImage( uint32_t inImageIndex, vk::Semaphore & inWaitSemaphore );
            std::vector< vk::Framebuffer > CreateFrameBuffers( vk::RenderPass inRenderPass );
            vk::Format GetImageFormat();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_SWAPCHAIN_H
