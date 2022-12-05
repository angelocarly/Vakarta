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
            SwapChain( vks::LogicalDevicePtr inLogicalDevice, vk::SurfaceKHR inSurface );
            ~SwapChain();

            void AcquireNextImage();
            vk::Format GetImageFormat();
            std::vector< vk::Image > GetSwapChainImages();
//            vk::Semaphore GetImageAvailableForPresentingSemaphore();
//            vk::Semaphore GetImageAvailableForRenderingSemaphore();

        private:
            vks::LogicalDevicePtr mLogicalDevice;
            vk::SurfaceKHR mSurface;
            vk::SwapchainKHR mSwapchain;

        private:
            vk::Semaphore mImageAvailableForRenderingSemophore;
            vk::Semaphore mImageAvailableForPresentingSemophore;
            vk::Fence mImageInFlightFence;

    };
}


#endif //VKRT_SWAPCHAIN_H
