//
// Created by Angelo Carly on 01/01/2023.
//

#ifndef VKRT_RENDERPASS_H
#define VKRT_RENDERPASS_H

#include "Swapchain.h"

namespace vks
{
    class RenderPass
    {
        public:
            RenderPass( vks::SwapchainPtr inSwapChain );
            ~RenderPass();

        public:
            vk::RenderPass GetVkRenderPass();
            vk::RenderPassBeginInfo GetVkBeginInfo( int inImageIndex );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_RENDERPASS_H
