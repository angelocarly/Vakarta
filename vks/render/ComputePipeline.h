//
// Created by Angelo Carly on 11/01/2023.
//

#ifndef VKRT_COMPUTEPIPELINE_H
#define VKRT_COMPUTEPIPELINE_H

#include "vks/render/ForwardDecl.h"
#include "DescriptorSet.h"

#include <memory>

namespace vks
{
    class ComputePipeline
    {
        public:
            struct ComputePipelineCreateInfo
            {
                vk::RenderPass mRenderPass;
                vk::ShaderModule mComputeShaderModule;
                std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
                std::vector< vk::PushConstantRange > mPushConstants;
            };

        public:
            ComputePipeline( vks::DevicePtr inDevice, ComputePipelineCreateInfo inPipelineCreateInfo );
            ~ComputePipeline();

        public:
            void Bind( vk::CommandBuffer inCommandBuffer );
            void BindDescriptorSets( vk::CommandBuffer inCommandBuffer, std::vector< vks::DescriptorSet > inDescriptorSets );

            vk::Pipeline GetVkPipeline();
            vk::PipelineLayout GetVkPipelineLayout();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_COMPUTEPIPELINE_H
