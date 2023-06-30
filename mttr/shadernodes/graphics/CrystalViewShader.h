/**
 * CrystalViewShader.h
 *
 * @file	CrystalViewShader.h
 * @author	Angelo Carly
 * @date	08/05/2023
 *
 */

#ifndef VKRT_CRYSTALVIEWSHADER_H
#define VKRT_CRYSTALVIEWSHADER_H

#include "vks/render/ForwardDecl.h"
#include "vks/core/Image.h"

#include <vulkan/vulkan.hpp>
#include <glm/vec3.hpp>

namespace vkrt::graphics
{
    class CrystalViewShader
    {
        public:
            CrystalViewShader( vks::VksSessionPtr inSession, vk::RenderPass inRenderPass );
            ~CrystalViewShader();

            void Bind( vk::CommandBuffer inCommandBuffer );
            void BindImage( vk::ImageView inOutputImageView );
            void Dispatch( vk::CommandBuffer inCommandBuffer, std::size_t inWidth, std::size_t inHeight, glm::vec3 inA, glm::vec3 inB, glm::vec3 inC, glm::vec3 inD, float inTScale );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif //VKRT_CRYSTALVIEWSHADER_H
