//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_RENDERER_H
#define VKRT_RENDERER_H

#include "vkrt/core/Camera.h"
#include "vkrt/core/ForwardDecl.h"
#include "vkrt/core/InputState.h"

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
#include "imgui_internal.h"
#include "MeshPipeline.h"

namespace vkrt
{
    class RendererConfig
    {
        public:
            enum Topology { TRIANGLES, LINES };

        public:
            Topology topology;
    };

    class Renderer
    {
        public:
            Renderer( vks::VulkanSessionPtr inSession, vks::WindowPtr inWindow, RendererConfig inConfig );
            ~Renderer();

            ImGuiContext * GetImGuiContext();

        public:
            void RenderFrame( vks::Mesh & inMesh );
            void SetCamera( vkrt::CameraPtr inCamera );
            RendererConfig GetConfig();
            void SetConfig( RendererConfig inConfig );

        private:
            vks::VulkanSessionPtr mSession;
            vks::DevicePtr mDevice;
            vkrt::RendererConfig mConfig;

            // Display
            vks::WindowPtr mWindow;
            vks::SwapchainPtr mSwapChain;
            std::vector< vk::CommandBuffer > mCommandBuffers;

            // Gui
            vks::ImGuiPtr mGui;

            // Pipeline
            vks::RenderPassPtr mRenderPass;
            std::unique_ptr< vkrt::MeshPipeline > mMeshPipeline;
            std::unique_ptr< vkrt::MeshPipeline > mMeshLinePipeline;

            // Scene
            vkrt::CameraPtr mCamera;

        private:
            void InitializeCommandBuffers();
    };
}


#endif //VKRT_RENDERER_H
