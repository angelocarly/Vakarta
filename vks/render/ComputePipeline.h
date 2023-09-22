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
                vk::ShaderModule mComputeShaderModule;
                std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
                std::vector< vk::PushConstantRange > mPushConstants;
                vk::SpecializationInfo mSpecializationInfo;
            };

        public:
            ComputePipeline( vks::DevicePtr inDevice, ComputePipelineCreateInfo inPipelineCreateInfo );
            ~ComputePipeline();

        public:
            void Bind( vk::CommandBuffer inCommandBuffer );
            void BindDescriptorSets( vk::CommandBuffer inCommandBuffer, std::vector< vks::DescriptorSet > inDescriptorSets );
            void PushConstants( vk::CommandBuffer inCommandBuffer, vk::DeviceSize theSize, const void * theMemory );
            void PushDescriptor( vk::CommandBuffer inCommandBuffer, vk::WriteDescriptorSet const & inWriteDescriptorSet );

            vk::Pipeline GetVkPipeline();
            vk::PipelineLayout GetVkPipelineLayout();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };

    class ComputePipelineBuilder
    {
        public:
            ComputePipelineBuilder( vks::DevicePtr inDevice );
            ~ComputePipelineBuilder();
            ComputePipelinePtr Build();

        public:
            ComputePipelineBuilder & SetComputeShader( vk::ShaderModule inComputeShaderModule );
            ComputePipelineBuilder & SetDescriptorSetLayouts( std::vector< vk::DescriptorSetLayout > inDescriptorSetLayouts );
            ComputePipelineBuilder & SetPushConstants( std::vector< vk::PushConstantRange > inPushConstants );
            ComputePipelineBuilder & SetSpecializationInfo( vk::SpecializationInfo inSpecializationInfo );

        private:
            vks::DevicePtr mDevice;
            ComputePipeline::ComputePipelineCreateInfo mPipelineCreateInfo;

    };
}


#endif //VKRT_COMPUTEPIPELINE_H
