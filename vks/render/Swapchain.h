//
// Created by Angelo Carly on 03/12/2022.
//

#ifndef VKRT_SWAPCHAIN_H
#define VKRT_SWAPCHAIN_H

#include "PhysicalDevice.h"
#include "ForwardDecl.h"

#include <vulkan/vulkan.hpp>
#include <cstdint>

namespace vks
{
    class Swapchain
    {
        public:
            Swapchain( vks::DevicePtr inLogicalDevice, vk::SurfaceKHR inSurface );
            ~Swapchain();

        public:
            vks::DevicePtr GetDevice();
            vk::Extent2D GetExtent() const;
            vk::Format GetImageFormat();
            std::vector< vk::ImageView > GetSwapchainImageViews() const;
            int GetImageCount() const;
            int GetMinImageCount() const;

            int RetrieveNextImage();
            void SubmitCommandBuffer( std::uint32_t inImageIndex, vk::CommandBuffer inCommandBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_SWAPCHAIN_H
