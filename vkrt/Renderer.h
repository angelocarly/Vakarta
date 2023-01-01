//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_RENDERER_H
#define VKRT_RENDERER_H

#include "vks/Device.h"
#include "vks/PhysicalDevice.h"
#include "vks/Swapchain.h"
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
            void InitializeCommandBuffers();
            void InitializeSynchronizationObject();
            void InitializeRenderObject();

        private:
            vks::Instance & mInstance;
            vks::PhysicalDevicePtr mPhysicalDevice;
            vks::DevicePtr mDevice;
            vk::Semaphore mPresentSemaphore;

            // Display
            vkrt::WindowPtr mWindow;
            vks::SwapchainPtr mSwapChain;
            std::vector< vk::CommandBuffer > mCommandBuffers;

            // Pipeline
            vks::RenderPassPtr mRenderPass;
            std::unique_ptr< vks::Pipeline > mPipeline;

            // Render data
            std::unique_ptr< vks::Mesh > mMesh;
    };
}


#endif //VKRT_RENDERER_H
