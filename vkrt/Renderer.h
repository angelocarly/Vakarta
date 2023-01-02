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
#include "vks/Window.h"
#include "ForwardDecl.h"
#include "Camera.h"
#include "InputState.h"
#include "vks/Gui.h"

namespace vkrt
{
    class Renderer
    {
        public:
            Renderer( vks::WindowPtr inWindow );
            ~Renderer();

        public:
            void Render();
            void SetCamera( vkrt::CameraPtr inCamera );

        private:
            vks::Instance & mInstance;
            vks::PhysicalDevicePtr mPhysicalDevice;
            vks::DevicePtr mDevice;

            // Display
            vks::WindowPtr mWindow;
            vks::SwapchainPtr mSwapChain;
            std::vector< vk::CommandBuffer > mCommandBuffers;

            // Gui
            vks::ImGuiPtr mGui;

            // Pipeline
            vks::RenderPassPtr mRenderPass;
            std::unique_ptr< vks::Pipeline > mPipeline;

            // Render data
            std::unique_ptr< vks::Mesh > mMesh;

            // Scene
            vkrt::CameraPtr mCamera;

        private:
            void InitializeCommandBuffers();
            void InitializeRenderObject();
    };
}


#endif //VKRT_RENDERER_H
