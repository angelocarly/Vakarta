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
#include "Window.h"
#include "ForwardDecl.h"

namespace vkrt
{
    class Renderer
    {
        public:
            Renderer( vkrt::WindowPtr inWindow );
            ~Renderer();

            void Render();

        private:
            void InitializeRenderPass();
            void InitializeFrameBuffers();

        private:
            vks::Instance & mInstance;
            vks::PhysicalDevicePtr mPhysicalDevice;
            vks::DevicePtr mDevice;
            vk::Semaphore mPresentSemaphore;

            // Display
            vkrt::WindowPtr mWindow;
            vks::SwapChain mSwapChain;
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
