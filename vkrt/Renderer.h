//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_RENDERER_H
#define VKRT_RENDERER_H

#include "Camera.h"
#include "ForwardDecl.h"
#include "InputState.h"

#include "vks/render/Device.h"
#include "vks/render/PhysicalDevice.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Mesh.h"
#include "vks/render/Window.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/GuiPass.h"
#include "vks/assets/AssetLoader.h"
#include "vks/render/VulkanSession.h"

namespace vkrt
{
    class Renderer
    {
        public:
            Renderer( vks::VulkanSessionPtr inSession, vks::WindowPtr inWindow );
            ~Renderer();

        public:
            void Render( vks::Mesh & inMesh );
            void SetCamera( vkrt::CameraPtr inCamera );

        private:
            vks::VulkanSessionPtr mSession;
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

            // Scene
            vkrt::CameraPtr mCamera;

        private:
            void InitializeCommandBuffers();
            void InitializeRenderObject();
    };
}


#endif //VKRT_RENDERER_H
