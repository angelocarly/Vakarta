//
// Created by magnias on 6/4/23.
//

#ifndef VKRT_RENDERENVIRONMENT_H
#define VKRT_RENDERENVIRONMENT_H

#include "vks/core/Image.h"

#include <vulkan/vulkan.hpp>

namespace vkrt
{
    class RenderEnvironment
    {
        public:
            std::size_t mFrameIndex;
            vk::Rect2D mResolution;
            vk::CommandBuffer mCommandBuffer;
            vk::RenderPass mRenderPass;
    };
}

#endif
