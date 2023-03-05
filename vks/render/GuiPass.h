//
// Created by Angelo Carly on 02/01/2023.
//

#ifndef VKRT_GUIPASS_H
#define VKRT_GUIPASS_H

#include "ForwardDecl.h"
#include "imgui_internal.h"

#include <vulkan/vulkan.hpp>

#include <memory>

namespace vks
{
    class GuiPass
    {
        public:
            GuiPass( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr inSwapChain );
            ~GuiPass();

        public:
            void Render( vk::CommandBuffer inCommandBuffer );
            ImGuiContext * GetImGuiContext();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_GUIPASS_H
