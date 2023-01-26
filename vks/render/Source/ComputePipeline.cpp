//
// Created by Angelo Carly on 11/01/2023.
//

#include "vks/render/ComputePipeline.h"

#include "vks/core/Vertex.h"
#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorSet.h"

class vks::ComputePipeline::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vks::ComputePipeline::ComputePipelineCreateInfo inPipelineCreateInfo );
        ~Impl();

    public:
        void InitializePipeline();

    public:
        vks::DevicePtr mDevice;
        vks::ComputePipeline::ComputePipelineCreateInfo mPipelineCreateInfo;

    public:
        vk::PipelineLayout mPipelineLayout;
        vk::Pipeline mPipeline;
};

vks::ComputePipeline::Impl::Impl( vks::DevicePtr inDevice, vks::ComputePipeline::ComputePipelineCreateInfo inPipelineCreateInfo )
:
    mDevice( inDevice ),
    mPipelineCreateInfo( inPipelineCreateInfo )
{
    InitializePipeline();
}

vks::ComputePipeline::Impl::~Impl()
{
    mDevice->GetVkDevice().destroy( mPipelineLayout );
    mDevice->GetVkDevice().destroy( mPipeline );
}

void
vks::ComputePipeline::Impl::InitializePipeline()
{
    // Shader
    vk::PipelineShaderStageCreateInfo theComputeShaderStageCreateInfo = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eCompute,
        mPipelineCreateInfo.mComputeShaderModule,
        "main"
    );

    // Pipeline layout
    auto thePipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo
    (
        vk::PipelineLayoutCreateFlags(),
        mPipelineCreateInfo.mDescriptorSetLayouts.size(),
        mPipelineCreateInfo.mDescriptorSetLayouts.data(),
        mPipelineCreateInfo.mPushConstants.size(),
        mPipelineCreateInfo.mPushConstants.data()
    );
    mPipelineLayout = mDevice->GetVkDevice().createPipelineLayout
    (
        thePipelineLayoutCreateInfo
    );

    // Pipeline
    auto thePipelineCreateInfo = vk::ComputePipelineCreateInfo
    (
        vk::PipelineCreateFlags(),
        theComputeShaderStageCreateInfo,
        mPipelineLayout,
        mPipeline,
        0
    );

    mPipeline = static_cast< vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >(
        mDevice->GetVkDevice().createComputePipelineUnique
        (
            vk::PipelineCache(),
            thePipelineCreateInfo
        ).value
    ).release();
}

//======================================================================================================================

vks::ComputePipeline::ComputePipeline( vks::DevicePtr inDevice, vks::ComputePipeline::ComputePipelineCreateInfo inPipelineCreateInfo )
:
    mImpl( new Impl( inDevice, inPipelineCreateInfo ) )
{

}

vks::ComputePipeline::~ComputePipeline()
{

}

vk::Pipeline
vks::ComputePipeline::GetVkPipeline()
{
    return mImpl->mPipeline;
}

vk::PipelineLayout
vks::ComputePipeline::GetVkPipelineLayout()
{
    return mImpl->mPipelineLayout;
}

void
vks::ComputePipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mImpl->mPipeline );
}

void
vks::ComputePipeline::BindDescriptorSets( vk::CommandBuffer inCommandBuffer, std::vector< vks::DescriptorSet > inDescriptorSets )
{
    std::vector< vk::DescriptorSet > theVkDescriptorSets;
    for( vks::DescriptorSet theDescriptorSet : inDescriptorSets )
    {
        theVkDescriptorSets.push_back( theDescriptorSet.GetVkDescriptorSet() );
    }
    inCommandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eCompute, mImpl->mPipelineLayout, 0, theVkDescriptorSets, nullptr );
}

