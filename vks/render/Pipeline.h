//
// Created by Angelo Carly on 11/01/2023.
//

#ifndef VKRT_PIPELINE_H
#define VKRT_PIPELINE_H

#include "vks/render/ForwardDecl.h"
#include "DescriptorSet.h"

#include <memory>

namespace vks
{
    class Pipeline
    {
        public:
            struct PipelineCreateInfo
            {
                vk::RenderPass mRenderPass;
                vk::ShaderModule mVertexShaderModule;
                vk::ShaderModule mFragmentShaderModule;
                std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
                std::vector< vk::PushConstantRange > mPushConstants;
            };
            struct PipelineConfigInfo
            {
                vk::PrimitiveTopology topology;
                std::vector< vk::VertexInputBindingDescription > mVertexInputBindingDescriptions;
                std::vector< vk::VertexInputAttributeDescription > mVertexInputAttributeDescriptions;
            };

        public:
            Pipeline( vks::DevicePtr inDevice, PipelineCreateInfo inPipelineCreateInfo, PipelineConfigInfo inPipelineConfigInfo );
            ~Pipeline();

        public:
            void Bind( vk::CommandBuffer inCommandBuffer );
            void BindDescriptorSets( vk::CommandBuffer inCommandBuffer, std::vector< vks::DescriptorSet > inDescriptorSets );

            vk::Pipeline GetVkPipeline();
            vk::PipelineLayout GetVkPipelineLayout();
            PipelineConfigInfo GetDefaultConfig();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_PIPELINE_H
