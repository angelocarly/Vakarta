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
    class Swapchain
    {
        public:
            Swapchain( vks::DevicePtr inLogicalDevice, vk::SurfaceKHR inSurface );
            ~Swapchain();

            vks::DevicePtr GetDevice();
            vk::Extent2D GetExtent();
            vk::SwapchainKHR GetVkSwapchain();
            vk::Format GetImageFormat();
            std::vector< vk::ImageView > GetSwapchainImageViews();

            int GetImageCount();
            int RetrieveNextImage();
            void PresentImage( uint32_t inImageIndex, vk::Semaphore & inWaitSemaphore );
            void SubmitCommandBuffer( uint32_t inImageIndex, vk::CommandBuffer inCommandBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_SWAPCHAIN_H
