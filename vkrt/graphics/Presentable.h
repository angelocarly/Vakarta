/**
 * Presentable.h
 *
 * @file	Presentable.h
 * @author	Angelo Carly
 * @date	29/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Vkrt_Presentable_h
#define Vkrt_Presentable_h

#include "vkrt/graphics/RenderEnvironment.h"

#include <vulkan/vulkan.hpp>

namespace vkrt
{
    class Presentable
    {
        public:
            virtual void Draw( vkrt::RenderEnvironment const & inRenderEnvironment ) = 0;
            virtual vk::ImageView GetImageView() const = 0;
            virtual vk::Sampler GetSampler() const = 0;
    };
}

#endif
