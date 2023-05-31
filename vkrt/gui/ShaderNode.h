/**
 * vkrt/gui/ShaderNode.h
 *
 * A node that can execute a custom shader.
 *
 * @file	ShaderNode.h
 * @author	Angelo Carly
 * @date	12/05/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Vkrt_Gui_ShaderNode_h_Angelo
#define Vkrt_Gui_ShaderNode_h_Angelo

#include "Node.h"

#include "vkrt/graphics/CrystalViewShader.h"
#include "vkrt/graphics/ForwardDecl.h"

#include "vks/core/Image.h"

namespace vkrt::gui
{
    class ShaderNode : public Node
    {
        public:
            ShaderNode( NodeContextPtr inContext );
            ~ShaderNode();

            void Draw() override;

        private:
            glm::ivec2 mImageSize;

            // Vulkan handles
            vk::ImageView mImageView;
            vks::Image mImage;
            vk::RenderPass mRenderPass;
            vk::Framebuffer mFramebuffer;

            // Shader inputs
            float mSliderValue;
            float mSliderValue2;
            glm::vec3 mAColor;
            glm::vec3 mBColor;
            glm::vec3 mCColor;
            glm::vec3 mDColor;
            float mTScale;

            // Shader
            vkrt::graphics::CrystalViewShaderPtr mPipeline;

            // Output data
            std::shared_ptr< vkrt::gui::OutputAttribute< vks::Image > > mOutput;

        private:
            void InitializeRenderTargets();
    };
}


#endif //VKRT_SHADERNODE_H
