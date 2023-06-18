//
// Created by Angelo Carly on 23/11/2022.
//

#ifndef VKRT_DISPLAY_H
#define VKRT_DISPLAY_H

#include "vkrt/core/Camera.h"
#include "vkrt/core/ForwardDecl.h"
#include "vkrt/core/InputState.h"

#include "vkrt/graphics/Presenter.h"

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

            vks::SwapchainPtr GetSwapchain() const { return mSwapchain; }

        public:
            void SetPresenter( std::shared_ptr< Presenter > inPresenter );
            void Render();

        private:
            void InitializeCommandBuffers();
            void InitializeRenderPass();
            void InitializeFrameBuffers();
            void InitializePipeline( vk::RenderPass inRenderPass );

            vk::RenderPassBeginInfo CreateRenderPassBeginInfo( std::size_t inFrameIndex );

        private:
            vks::VulkanSessionPtr mSession;
            vks::DevicePtr mDevice;

            vks::WindowPtr mWindow;
            vks::SwapchainPtr mSwapchain;
            vks::PipelinePtr mPipeline;

            vk::RenderPass mRenderPass;
            vk::ClearValue mClearValue;

            // Per frame data
            std::vector< vk::CommandBuffer > mCommandBuffers;
            std::vector< vk::Framebuffer > mFrameBuffers;

            // Presenter
            std::shared_ptr< Presenter > mPresenter;
    };
}

#endif
