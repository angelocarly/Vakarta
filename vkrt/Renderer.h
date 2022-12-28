//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_RENDERER_H
#define VKRT_RENDERER_H

#include "vks/Device.h"
#include "vks/PhysicalDevice.h"
#include "vks/SwapChain.h"
#include "vks/Pipeline.h"
#include "vks/Mesh.h"

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
            void InitializeFrameBuffers();
            void RecordCommandBuffers();

        private:
            vks::Instance & mInstance;
            vks::PhysicalDevicePtr mPhysicalDevice;
            vks::DevicePtr mDevice;
            vk::Semaphore mPresentSemaphore;

            // Display
            vk::SurfaceKHR mSurface;
            vks::SwapChain mSwapChain;
            std::vector< vk::ImageView > mSwapChainImageViews;
            std::vector< vk::Framebuffer > mFrameBuffers;

            // Pipeline
//            vk::AttachmentReference mColorAttachmentReference;
//            vk::AttachmentDescription mColorAttachmentDescription;
//            vk::SubpassDescription mSubPass;
            vk::RenderPass mRenderPass;
            std::unique_ptr< vks::Pipeline > mPipeline;

            // Render data
            std::unique_ptr< vks::Mesh > mMesh;

            void InitializeSynchronizationObject();

            void InitializeRenderObject();
    };
}


#endif //VKRT_RENDERER_H
