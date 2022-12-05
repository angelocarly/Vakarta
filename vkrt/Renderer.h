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

            void Render();

        private:
            void CreateSwapChainImageViews();
            void InitializeRenderPass();

        private:
            vks::Instance & mInstance;
            vks::PhysicalDevicePtr mPhysicalDevice;
            vks::LogicalDevicePtr mLogicalDevice;

            // Display
            vk::SurfaceKHR mSurface;
            vks::SwapChain mSwapChain;
            std::vector< vk::ImageView > mSwapChainImageViews;

            // Pipeline
//            vk::AttachmentReference mColorAttachmentReference;
//            vk::AttachmentDescription mColorAttachmentDescription;
//            vk::SubpassDescription mSubPass;
            vk::RenderPass mRenderPass;

            // Commands
            vk::Queue mQueue;
    };
}


#endif //VKRT_RENDERER_H
