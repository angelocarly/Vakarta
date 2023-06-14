/**
 * OffScreenRenderPass.h
 *
 * @file	OffScreenRenderPass.h
 * @author	Angelo Carly
 * @date	09/05/2023
 *
 *
 */

#ifndef VKRT_OFFSCREENRENDERPASS_H
#define VKRT_OFFSCREENRENDERPASS_H

#include "vks/render/ForwardDecl.h"

#include <vulkan/vulkan.hpp>

namespace vks
{
    class OffScreenRenderPass
    {
        public:
            OffScreenRenderPass( vks::DevicePtr inDevice, std::size_t inConcurrentCount, vk::Format inFormat, std::size_t inWidth, std::size_t inHeight );
            ~OffScreenRenderPass();


        public:
            vk::RenderPass GetVkRenderPass();
            vk::RenderPassBeginInfo GetVkBeginInfo( int inImageIndex );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_OFFSCREENRENDERPASS_H
