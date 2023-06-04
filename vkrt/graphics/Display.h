//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_DISPLAY_H
#define VKRT_DISPLAY_H

#include "vkrt/core/Camera.h"
#include "vkrt/core/ForwardDecl.h"
#include "vkrt/core/InputState.h"

#include "vkrt/graphics/Layer.h"

#include "vks/assets/AssetLoader.h"
#include "vks/render/Device.h"
#include "vks/render/PhysicalDevice.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Mesh.h"
#include "vks/render/Window.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/GuiPass.h"
#include "vks/render/VksSession.h"

#include "imgui_internal.h"
#include "ForwardDecl.h"
#include "RenderEnvironment.h"

namespace vkrt
{
    /**
     * @brief Responsible for rendering separate layers to the screen
     */
    class Display
    {
        public:
            Display( vks::VulkanSessionPtr inSession, vks::WindowPtr inWindow );
            ~Display();

            ImGuiContext * GetImGuiContext();

        public:
            void RegisterLayer( vkrt::LayerPtr inLayer );
            void Render();

        private:
            void InitializePipeline();

        private:
            vks::VulkanSessionPtr mSession;
            vks::DevicePtr mDevice;

            vks::WindowPtr mWindow;
            vks::SwapchainPtr mSwapChain;
            vks::RenderPassPtr mRenderPass;

            vk::DescriptorSetLayout mImageDescriptorLayout;
            vks::PipelinePtr mDisplayPipeline;

            struct LayerContext
            {
                vk::ImageLayout mImageLayout;
                vks::Image mImage;
                vk::ImageView mImageView;
                vk::Sampler mSampler;
                vkrt::LayerPtr mLayer;
            };

            struct FrameContext
            {
                std::vector< LayerContext > mLayers;
                vk::CommandBuffer mCommandBuffer;
            };

            std::vector< FrameContext > mFrameContext;
    };
}

#endif
