/**
 * ImageProvider.h
 *
 * @file	ImageProvider.h
 * @author	Angelo Carly
 * @date	25/04/2023
 *
 *
 */

#ifndef VKRT_IMAGEPROVIDER_H
#define VKRT_IMAGEPROVIDER_H

#include <vulkan/vulkan.hpp>

namespace vkrt::gui
{
    class ImageProvider
    {
        public:
            ImageProvider() = default;
            virtual ~ImageProvider() = default;

            virtual vk::DescriptorSet ProvideImage() = 0;
    };
}

#endif //VKRT_IMAGEPROVIDER_H
