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

            int AcquireNextImage();
            vk::Format GetImageFormat();
            std::vector< vk::Image > GetSwapChainImages();
            std::vector< vk::ImageView > GetSwapChainImageViews();
            vk::SwapchainKHR GetVkSwapchain();
            void PresentSwapChain( uint32_t inImageIndex, vk::Semaphore & inWaitSemaphore );

        private:
            vks::DevicePtr mDevice;
            vk::SurfaceKHR mSurface;
            vk::SwapchainKHR mSwapchain;

        private:
            void CreateSwapChainImages();
            vk::Semaphore mImageAvailableForRenderingSemophore;
            vk::Semaphore mImageAvailableForPresentingSemophore;
            vk::Fence mImageInFlightFence;
            std::vector< vk::Image > mSwapChainImages;
            std::vector< vk::ImageView > mSwapChainImageViews;

    };
}


#endif //VKRT_SWAPCHAIN_H
