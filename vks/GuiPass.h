//
// Created by Angelo Carly on 02/01/2023.
//

#ifndef VKRT_GUI_H
#define VKRT_GUI_H

#include "ForwardDecl.h"

#include <memory>

namespace vks
{
    class Gui
    {
        public:
            Gui( vks::DevicePtr inDevice, vks::WindowPtr inWindow, vks::RenderPassPtr inRenderPass, vks::SwapchainPtr inSwapChain );
            ~Gui();

        public:
            void Render( vk::CommandBuffer inCommandBuffer );
            void Update();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_GUI_H
