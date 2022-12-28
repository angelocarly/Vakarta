//
// Created by Angelo Carly on 18/12/2022.
//

#include "vks/Pipeline.h"

#include "vks/ForwardDecl.h"
#include "vks/Device.h"
#include "vks/Vertex.h"
#include "vks/Utils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.hpp>

struct UniformBufferObject
{
    glm::mat4 mView;
};

class vks::Pipeline::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vk::RenderPass inRenderPass );
        ~Impl();

    private:
        void CreateBuffers();
        void InitializeDescriptors();
        void InitializePipeline();

    public:
        vks::DevicePtr mDevice;
        vk::RenderPass mRenderPass;

        vk::PipelineLayout mPipelineLayout;
        vk::Pipeline mPipeline;

        vk::DescriptorPool mDescriptorPool;
        std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
        vk::DescriptorSet mDescriptorSet;
        vks::Buffer mUniformBuffer;
};

void UpdatePipelineUniforms();

vks::Pipeline::Impl::Impl( vks::DevicePtr inDevice, vk::RenderPass inRenderPass )
:
    mDevice( inDevice ),
    mRenderPass( inRenderPass )
{
    CreateBuffers();
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
vks::Pipeline::Impl::CreateBuffers()
{
    // Pipeline uniform buffer
    vma::AllocationCreateInfo theUniformBufferAllocationInfo;
    theUniformBufferAllocationInfo.usage = vma::MemoryUsage::eAuto;
    theUniformBufferAllocationInfo.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
    mUniformBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo
        (
            vk::BufferCreateFlags(),
            sizeof( UniformBufferObject ),
            vk::BufferUsageFlagBits::eUniformBuffer
        ),
        theUniformBufferAllocationInfo
    );
}

void
vks::Pipeline::Impl::InitializeDescriptors()
{
    // Layout
    std::vector< vk::DescriptorSetLayoutBinding > theDescriptorSetLayoutBindings =
    {
        vk::DescriptorSetLayoutBinding
        (
            std::uint32_t( 0 ),
            vk::DescriptorType::eUniformBuffer,
            1,
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

    // Pool
    std::array< vk::DescriptorPoolSize, 1 > thePoolSizes = {};
    thePoolSizes[ 0 ].setType( vk::DescriptorType::eUniformBuffer );
    thePoolSizes[ 0 ].setDescriptorCount( 1 );

    vk::DescriptorPoolCreateInfo const theDescriptorPoolCreateInfo
        (
            vk::DescriptorPoolCreateFlags(),
            1,
            thePoolSizes.size(),
            thePoolSizes.data()
        );

    mDescriptorPool = mDevice->GetVkDevice().createDescriptorPool( theDescriptorPoolCreateInfo );

    // Allocate a descriptor set from the pool
    vk::DescriptorSetAllocateInfo const theDescriptorSetAllocateInfo
    (
        mDescriptorPool,
        std::uint32_t( mDescriptorSetLayouts.size() ),
        mDescriptorSetLayouts.data()
    );

    mDescriptorSet = mDevice->GetVkDevice().allocateDescriptorSets( theDescriptorSetAllocateInfo ).front();

    // Configure the descriptors
    std::array< vk::WriteDescriptorSet, 1 > theWriteDescriptorSet;

    // Uniform buffer
    vk::DescriptorBufferInfo const theDescriptorBufferInfo
    (
        mUniformBuffer.GetVkBuffer(),
        0,
        sizeof( UniformBufferObject )
    );
    theWriteDescriptorSet[ 0 ].setDstSet( mDescriptorSet );
    theWriteDescriptorSet[ 0 ].setDstBinding( 0 );
    theWriteDescriptorSet[ 0 ].setDstArrayElement( 0 );
    theWriteDescriptorSet[ 0 ].setDescriptorType( vk::DescriptorType::eUniformBuffer );
    theWriteDescriptorSet[ 0 ].setDescriptorCount( 1 );
    theWriteDescriptorSet[ 0 ].setPBufferInfo( &theDescriptorBufferInfo );

    mDevice->GetVkDevice().updateDescriptorSets( theWriteDescriptorSet, {} );

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
    vk::PipelineViewportStateCreateInfo const theViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo
    (
        vk::PipelineViewportStateCreateFlags(),
        0,
        nullptr,
        0,
        nullptr
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
vks::Pipeline::UpdatePipelineUniforms( int inWidth, int inHeight )
{
    glm::mat4 theView = glm::lookAt
    (
        glm::vec3(0,0,-10), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    auto theProjection = glm::perspective(glm::radians(90.0f ), (float) inWidth / (float) inHeight, 0.1f, 100.0f );

    UniformBufferObject theUniform;
    theUniform.mView = theProjection = theProjection * theView;

    void * theData = mImpl->mDevice->MapMemory( mImpl->mUniformBuffer );
    std::memcpy( theData, & theUniform, sizeof( UniformBufferObject ) );
    mImpl->mDevice->UnmapMemory( mImpl->mUniformBuffer );
}

void
vks::Pipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, mImpl->mPipelineLayout, 0, mImpl->mDescriptorSet, nullptr );
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mImpl->mPipeline );
}