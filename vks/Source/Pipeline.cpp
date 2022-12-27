//
// Created by Angelo Carly on 18/12/2022.
//

#include "vks/Pipeline.h"

#include "vks/ForwardDecl.h"
#include "vks/LogicalDevice.h"
#include "vks/Vertex.h"
#include "vks/Utils.h"

#include <vulkan/vulkan.hpp>

class vks::Pipeline::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vk::RenderPass inRenderPass );
        ~Impl();

    private:
        void InitializeDescriptors();
        void InitializePipeline();

    public:
        vks::DevicePtr mDevice;
        vk::RenderPass mRenderPass;

        uint32_t mWidth;
        uint32_t mHeigth;

        vk::PipelineLayout mPipelineLayout;
        vk::Pipeline mPipeline;
        std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
};

vks::Pipeline::Impl::Impl( vks::DevicePtr inDevice, vk::RenderPass inRenderPass )
:
    mDevice( inDevice ),
    mRenderPass( inRenderPass ),
    mWidth( 1000 ),
    mHeigth( 1000 )
{
	InitializeDescriptors();
	InitializePipeline();
}

vks::Pipeline::Impl::~Impl()
{
    for( auto theDescriptorSetLayout : mDescriptorSetLayouts )
    {
        mDevice->GetVkDevice().destroy( theDescriptorSetLayout );
    }

    mDevice->GetVkDevice().destroy( mPipelineLayout );
    mDevice->GetVkDevice().destroy( mPipeline );
}

void
vks::Pipeline::Impl::InitializeDescriptors()
{
    std::vector< vk::DescriptorSetLayoutBinding > theDescriptorSetLayoutBindings =
    {
        vk::DescriptorSetLayoutBinding
        (
            std::uint32_t( 0 ),
            vk::DescriptorType::eUniformBuffer,
            0,
            vk::ShaderStageFlagBits::eVertex,
            nullptr
        )
    };

    auto theDescriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo
    (
        vk::DescriptorSetLayoutCreateFlags(),
        theDescriptorSetLayoutBindings.size(),
        theDescriptorSetLayoutBindings.data()
    );

	auto theDescriptorSetLayout = mDevice->GetVkDevice().createDescriptorSetLayout
    (
        theDescriptorSetLayoutCreateInfo
    );
    mDescriptorSetLayouts = { theDescriptorSetLayout };

}

void
vks::Pipeline::Impl::InitializePipeline()
{
    // TODO: Move load shader out of vkrt
    // Shaders
    vk::ShaderModule theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, std::filesystem::path( "shaders/RenderShader.vert.spv" ) );
    vk::ShaderModule theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, std::filesystem::path( "shaders/RenderShader.frag.spv" ) );

    std::array< vk::PipelineShaderStageCreateInfo, 2 > thePipelineShaderStageCreateInfos;
    thePipelineShaderStageCreateInfos[ 0 ] = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eVertex,
        theVertexShader,
        "main"
    );

    thePipelineShaderStageCreateInfos[ 1 ] = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eFragment,
        theFragmentShader,
        "main"
    );

    // Vertex binding
    auto theVertexInputBindingDescription = vks::Vertex::GetVkVertexInputBindingDescription();
    auto theVertexInputAttributeDescription = vks::Vertex::GetVkVertexInputAttributeDescription();
    vk::PipelineVertexInputStateCreateInfo const theVertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo
    (
        vk::PipelineVertexInputStateCreateFlags(),
        1,
        & theVertexInputBindingDescription,
        1,
        & theVertexInputAttributeDescription
    );

    // Input assembly
    vk::PipelineInputAssemblyStateCreateInfo const theInputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo
    (
        vk::PipelineInputAssemblyStateCreateFlags(),
        vk::PrimitiveTopology::eTriangleList,
        VK_FALSE
    );

    // Tesselation
    vk::PipelineTessellationStateCreateInfo const theTessellationStateCreateInfo = vk::PipelineTessellationStateCreateInfo
    (
        vk::PipelineTessellationStateCreateFlags()
    );

    // Viewport
    vk::Viewport theViewport = vk::Viewport( 0, 0, mWidth, mHeigth );
    vk::Rect2D theScissors = vk::Rect2D( vk::Offset2D( 0, 0 ), vk::Extent2D( mWidth, mHeigth ) );
    vk::PipelineViewportStateCreateInfo const theViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo
    (
        vk::PipelineViewportStateCreateFlags(),
        1,
        & theViewport,
        1,
        & theScissors
    );

    // Rasterization
    vk::PipelineRasterizationStateCreateInfo const theRasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo
    (
        vk::PipelineRasterizationStateCreateFlags(),
        false,
        false,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eNone,
        vk::FrontFace::eClockwise,
        false,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );

    // Depth stencil
    vk::PipelineDepthStencilStateCreateInfo const theDepthStencilStateCreateInfo = vk::PipelineDepthStencilStateCreateInfo
    (
        vk::PipelineDepthStencilStateCreateFlags(),
        false,
        false,
        vk::CompareOp::eNever,
        false,
        false,
        vk::StencilOp::eKeep,
        vk::StencilOp::eKeep,
        0.1f,
        100.0f
    );

    // Color blend
    std::vector< vk::PipelineColorBlendAttachmentState > theColorBlendAttachmentStates =
    {
        vk::PipelineColorBlendAttachmentState
		(
			false
		)
    };
    vk::PipelineColorBlendStateCreateInfo const theColorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo
    (
        vk::PipelineColorBlendStateCreateFlags(),
        false,
        vk::LogicOp::eNoOp,
        theColorBlendAttachmentStates.size(),
        theColorBlendAttachmentStates.data(),
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    // Dynamic states
    std::vector< vk::DynamicState > theDynamicStates =
    {
        vk::DynamicState( VK_DYNAMIC_STATE_VIEWPORT )
    };
    vk::PipelineDynamicStateCreateInfo const theDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo
    (
        vk::PipelineDynamicStateCreateFlags(),
        theDynamicStates.size(),
        theDynamicStates.data()
    );

    // Pipeline Layout
    auto thePipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo
    (
        vk::PipelineLayoutCreateFlags(),
        mDescriptorSetLayouts.size(),
        mDescriptorSetLayouts.data(),
        0,
        nullptr
    );
    mPipelineLayout = mDevice->GetVkDevice().createPipelineLayout
    (
        thePipelineLayoutCreateInfo
    );

    // Pipeline
    auto thePipelineCreateInfo = vk::GraphicsPipelineCreateInfo
    (
        vk::PipelineCreateFlags(),
        1,
        thePipelineShaderStageCreateInfos.data(),
        & theVertexInputStateCreateInfo,
        & theInputAssemblyStateCreateInfo,
        & theTessellationStateCreateInfo,
        & theViewportStateCreateInfo,
        & theRasterizationStateCreateInfo,
        nullptr,
        & theDepthStencilStateCreateInfo,
        & theColorBlendStateCreateInfo,
        & theDynamicStateCreateInfo,
        mPipelineLayout,
        mRenderPass,
        0,
        mPipeline,
        0
    );

    mPipeline = static_cast< vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >(
        mDevice->GetVkDevice().createGraphicsPipelineUnique
        (
            vk::PipelineCache(),
            thePipelineCreateInfo
        ).value
    ).release();

    mDevice->GetVkDevice().destroy( theVertexShader );
    mDevice->GetVkDevice().destroy( theFragmentShader );
}

// =====================================================================================================================

vks::Pipeline::Pipeline( vks::DevicePtr inDevice, vk::RenderPass inRenderPass )
:
    mImpl( new Impl( inDevice, inRenderPass ) )
{

}

vks::Pipeline::~Pipeline()
{

}

void
vks::Pipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mImpl->mPipeline );
}