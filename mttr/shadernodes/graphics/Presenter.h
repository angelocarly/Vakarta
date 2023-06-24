/**
 * Presenter.h
 *
 * @file	Presenter.h
 * @author	Angelo Carly
 * @date	10/04/2023
 *
 *
 */

#ifndef VKRT_GRAPHICS_PRESENTER_H
#define VKRT_GRAPHICS_PRESENTER_H

#include "vks/core/ForwardDecl.h"

#include <vulkan/vulkan.hpp>

namespace vkrt
{
    class Presenter
    {
        public:
            virtual vk::DescriptorSet GetImage() = 0;
    };
}

#endif //VKRT_PRESENTER_H
