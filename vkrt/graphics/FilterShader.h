/**
 * FilterShader.h
 *
 * @file	FilterShader.h
 * @author	Angelo Carly
 * @date	08/05/2023
 *
 */

#ifndef VKRT_FilterShader_H
#define VKRT_FilterShader_H

#include "vks/render/ForwardDecl.h"
#include "vks/core/Image.h"

#include <vulkan/vulkan.hpp>
#include <glm/vec3.hpp>

namespace vkrt::graphics
{
    class FilterShader
    {
        public:
            FilterShader( vks::VulkanSessionPtr inSession, vk::RenderPass inRenderPass );
            ~FilterShader();

            void Bind( vk::CommandBuffer inCommandBuffer );
            void BindImage( vk::ImageView inInputImageView, vk::ImageView inOutputImageView );
            void Dispatch( vk::CommandBuffer inCommandBuffer, std::size_t inWidth, std::size_t inHeight );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_FilterShader_H
