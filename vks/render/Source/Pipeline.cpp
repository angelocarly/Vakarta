//
// Created by Angelo Carly on 11/01/2023.
//

#include "vks/render/Pipeline.h"

#include "vks/core/Vertex.h"
#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorSet.h"

class vks::Pipeline::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vks::Pipeline::PipelineCreateInfo inPipelineCreateInfo, vks::Pipeline::PipelineConfigInfo inPipelineConfigInfo );
        ~Impl();

    public:
        void InitializePipeline();

    public:
        vks::DevicePtr mDevice;
        vks::Pipeline::PipelineCreateInfo mPipelineCreateInfo;
        vks::Pipeline::PipelineConfigInfo mPipelineConfigInfo;

    public:
        vk::PipelineLayout mPipelineLayout;
        vk::Pipeline mPipeline;
};

vks::Pipeline::Impl::Impl( vks::DevicePtr inDevice, vks::Pipeline::PipelineCreateInfo inPipelineCreateInfo, vks::Pipeline::PipelineConfigInfo inPipelineConfigInfo )
:
    mDevice( inDevice ),
    mPipelineCreateInfo( inPipelineCreateInfo ),
    mPipelineConfigInfo( inPipelineConfigInfo )
{
    InitializePipeline();
}

vks::Pipeline::Impl::~Impl()
{
    mDevice->GetVkDevice().destroy( mPipelineLayout );
    mDevice->GetVkDevice().destroy( mPipeline );
}

void
vks::Pipeline::Impl::InitializePipeline()
{
    std::array< vk::PipelineShaderStageCreateInfo, 2 > thePipelineShaderStageCreateInfos;
    thePipelineShaderStageCreateInfos[ 0 ] = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eVertex,
        mPipelineCreateInfo.mVertexShaderModule,
        "main"
    );

    thePipelineShaderStageCreateInfos[ 1 ] = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eFragment,
        mPipelineCreateInfo.mFragmentShaderModule,
        "main"
    );

    // Vertex binding
    auto theVertexInputBindingDescriptions = vks::Vertex::GetVkVertexInputBindingDescriptions();
    auto theVertexInputAttributeDescriptions = vks::Vertex::GetVkVertexInputAttributeDescriptions();
    auto const theVertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo
    (
        vk::PipelineVertexInputStateCreateFlags(),
        theVertexInputBindingDescriptions.size(),
        theVertexInputBindingDescriptions.data(),
        theVertexInputAttributeDescriptions.size(),
        theVertexInputAttributeDescriptions.data()
    );

    // Input assembly
    auto const theInputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo
    (
        vk::PipelineInputAssemblyStateCreateFlags(),
        mPipelineConfigInfo.topology,
        VK_FALSE
    );

    // Viewport
    auto const theViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo
    (
        vk::PipelineViewportStateCreateFlags(),
        0,
        nullptr,
        0,
        nullptr
    );

    // Rasterization
    auto const theRasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo
    (
        vk::PipelineRasterizationStateCreateFlags(),
        false,
        false,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eNone,
        vk::FrontFace::eCounterClockwise,
        false,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );

    // Multi sample
    auto const theMultiSampleStateCreateInfo = vk::PipelineMultisampleStateCreateInfo
    (
        vk::PipelineMultisampleStateCreateFlags(),
        vk::SampleCountFlagBits::e1,
        VK_FALSE
    );

    // Depth stencil
    auto const theDepthStencilStateCreateInfo = vk::PipelineDepthStencilStateCreateInfo
    (
        vk::PipelineDepthStencilStateCreateFlags(),
        true,
        true,
        vk::CompareOp::eLess,
        false,
        false,
        vk::StencilOp::eKeep,
        vk::StencilOp::eKeep,
        0.0f,
        1.0f
    );

    // Color blend
    std::vector< vk::PipelineColorBlendAttachmentState > theColorBlendAttachmentStates =
    {
        vk::PipelineColorBlendAttachmentState
            (
                false,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eZero,
                vk::BlendOp::eAdd,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eZero,
                vk::BlendOp::eAdd,
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
            )
    };
    auto const theColorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo
    (
        vk::PipelineColorBlendStateCreateFlags(),
        false,
        vk::LogicOp::eCopy,
        theColorBlendAttachmentStates.size(),
        theColorBlendAttachmentStates.data(),
        { 0.0f, 0.0f, 0.0f, 0.0f }
    );

    // Dynamic states
    std::vector< vk::DynamicState > theDynamicStates =
    {
        vk::DynamicState( VK_DYNAMIC_STATE_VIEWPORT ),
        vk::DynamicState( VK_DYNAMIC_STATE_SCISSOR ),
    };
    auto const theDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo
    (
        vk::PipelineDynamicStateCreateFlags(),
        theDynamicStates.size(),
        theDynamicStates.data()
    );

    // Pipeline Layout
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
    auto thePipelineCreateInfo = vk::GraphicsPipelineCreateInfo
    (
        vk::PipelineCreateFlags(),
        thePipelineShaderStageCreateInfos.size(),
        thePipelineShaderStageCreateInfos.data(),
        &theVertexInputStateCreateInfo,
        &theInputAssemblyStateCreateInfo,
        nullptr,
        &theViewportStateCreateInfo,
        &theRasterizationStateCreateInfo,
        &theMultiSampleStateCreateInfo,
        &theDepthStencilStateCreateInfo,
        &theColorBlendStateCreateInfo,
        &theDynamicStateCreateInfo,
        mPipelineLayout,
        mPipelineCreateInfo.mRenderPass,
        0,
        nullptr,
        -1
    );

    mPipeline = static_cast< vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >(
        mDevice->GetVkDevice().createGraphicsPipelineUnique
        (
            vk::PipelineCache(),
            thePipelineCreateInfo
        ).value
    ).release();
}

//======================================================================================================================

vks::Pipeline::Pipeline( vks::DevicePtr inDevice, vks::Pipeline::PipelineCreateInfo inPipelineCreateInfo, vks::Pipeline::PipelineConfigInfo inPipelineConfigInfo )
:
    mImpl( new Impl( inDevice, inPipelineCreateInfo, inPipelineConfigInfo ) )
{

}

vks::Pipeline::~Pipeline()
{

}

vks::Pipeline::PipelineConfigInfo
vks::Pipeline::GetDefaultConfig()
{
    return
    {

    };
}

vk::Pipeline
vks::Pipeline::GetVkPipeline()
{
    return mImpl->mPipeline;
}

vk::PipelineLayout
vks::Pipeline::GetVkPipelineLayout()
{
    return mImpl->mPipelineLayout;
}

void
vks::Pipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mImpl->mPipeline );
}

void
vks::Pipeline::BindDescriptorSets( vk::CommandBuffer inCommandBuffer, std::vector< vks::DescriptorSet > inDescriptorSets )
{
    std::vector< vk::DescriptorSet > theVkDescriptorSets;
    for( vks::DescriptorSet theDescriptorSet : inDescriptorSets )
    {
        theVkDescriptorSets.push_back( theDescriptorSet.GetVkDescriptorSet() );
    }
    inCommandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, mImpl->mPipelineLayout, 0, theVkDescriptorSets, nullptr );
}

