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
            vks::Image mDisplayImage;
            vk::Rect2D mResolution;
            vk::ImageView mDisplayImageView;
            vk::CommandBuffer mCommandBuffer;
    };
}

#endif
