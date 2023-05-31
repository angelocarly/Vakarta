/**
 * vkrt/gui/FilterNode.h
 *
 * A node that can execute a custom shader.
 *
 * @file	FilterNode.h
 * @author	Angelo Carly
 * @date	12/05/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Vkrt_Gui_FilterNode_h_Angelo
#define Vkrt_Gui_FilterNode_h_Angelo

#include "Node.h"

#include "vkrt/graphics/CrystalViewShader.h"
#include "vkrt/graphics/ForwardDecl.h"

#include "vks/core/Image.h"

namespace vkrt::gui
{
    class FilterNode : public Node
    {
        public:
            FilterNode( NodeContextPtr inContext );
            ~FilterNode();

            void Draw() override;

        private:
            void SetupImageViews( std::optional< vks::Image > inImage );
            void DisconnectImageViews();

        private:
            glm::ivec2 mImageSize;

            // Vulkan handles
            vk::ImageView mImageView;
            vks::Image mImage;
            vk::RenderPass mRenderPass;
            vk::Framebuffer mFramebuffer;

            // Shader
            vkrt::graphics::FilterShaderPtr mPipeline;

            vk::ImageView mInputImageView;
            vk::Sampler mInputSampler;
            vk::DescriptorSet mInputDescriptorSet;
            std::shared_ptr< vkrt::gui::InputAttribute< vks::Image > > mInput;

            // Output data
            std::shared_ptr< vkrt::gui::OutputAttribute< vks::Image > > mOutput;

        private:
            void InitializeRenderTargets();
    };
}


#endif //VKRT_FilterNode_H
