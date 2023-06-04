//
// Created by Angelo Carly on 18/12/2022.
//

#include "vkrt/graphics/LinePipeline.h"

#include "vks/core/Vertex.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorLayoutBuilder.h"
#include "vks/render/DescriptorSet.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vulkan/vulkan.hpp>

struct UniformBufferObject
{
    glm::mat4 mView;
};

class vkrt::LinePipeline::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass );
        ~Impl();

    private:
        void CreateBuffers();
        void InitializeDescriptors();
        void InitializePipeline();

    public:
        vks::DevicePtr mDevice;
        vks::RenderPassPtr mRenderPass;
        vks::PipelinePtr mPipeline;

        // Descriptors
        vk::DescriptorPool mDescriptorPool;
        std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
        std::vector< vks::DescriptorSet > mDescriptorSets;
        vks::Buffer mUniformBuffer;
};

vkrt::LinePipeline::Impl::Impl( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass )
:
    mDevice( inDevice ),
    mRenderPass( inRenderPass )
{
    CreateBuffers();
    InitializeDescriptors();
    InitializePipeline();
}

vkrt::LinePipeline::Impl::~Impl()
{
    mDevice->DestroyBuffer( mUniformBuffer );

    for( auto theDescriptorSetLayout : mDescriptorSetLayouts )
    {
        mDevice->GetVkDevice().destroy( theDescriptorSetLayout );
    }
    mDevice->GetVkDevice().destroy( mDescriptorPool );
}

void
vkrt::LinePipeline::Impl::CreateBuffers()
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
vkrt::LinePipeline::Impl::InitializeDescriptors()
{
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

    // Layout
    auto theDescriptorLayoutBuilder = vks::DescriptorLayoutBuilder();
    theDescriptorLayoutBuilder.AddLayoutBinding( 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex );
    mDescriptorSetLayouts = theDescriptorLayoutBuilder.Build( mDevice );

    // Descriptor set
    auto theDescriptorSet = vks::DescriptorSet( mDevice, mDescriptorPool, mDescriptorSetLayouts.front() );
    theDescriptorSet.BindBuffer( 0, vk::DescriptorType::eUniformBuffer, mUniformBuffer );
    mDescriptorSets = { theDescriptorSet };
}

void
vkrt::LinePipeline::Impl::InitializePipeline()
{
    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/LineShader.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/LineShader.frag.spv" );

    vks::Pipeline::PipelineCreateInfo theCreateInfo =
    {
        mRenderPass->GetVkRenderPass(),
        theVertexShader,
        theFragmentShader,
        mDescriptorSetLayouts,
        {}
    };
    vks::Pipeline::PipelineConfigInfo theConfigInfo =
    {
        vk::PrimitiveTopology::eLineList,
        {
            vk::VertexInputBindingDescription
            (
                0,
                sizeof( glm::vec4 ),
                vk::VertexInputRate::eVertex
            )
        },
        {
            vk::VertexInputAttributeDescription
            (
                0,
                0,
                vk::Format::eR32G32B32A32Sfloat,
                0
            )
        }
    };
    mPipeline = std::make_unique< vks::Pipeline >( mDevice, theCreateInfo, theConfigInfo );

    mDevice->GetVkDevice().destroy( theVertexShader );
    mDevice->GetVkDevice().destroy( theFragmentShader );
}

// =====================================================================================================================

vkrt::LinePipeline::LinePipeline( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass )
:
    mImpl( new Impl( inDevice, inRenderPass ) )
{

}

vkrt::LinePipeline::~LinePipeline()
{

}

void
vkrt::LinePipeline::UpdatePipelineUniforms( glm::mat4 inCamera )
{
    UniformBufferObject theUniform;
    theUniform.mView = inCamera;

    void * theData = mImpl->mDevice->MapMemory( mImpl->mUniformBuffer );
    std::memcpy( theData, &theUniform, sizeof( UniformBufferObject ) );
    mImpl->mDevice->UnmapMemory( mImpl->mUniformBuffer );
}

void
vkrt::LinePipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    mImpl->mPipeline->BindDescriptorSets( inCommandBuffer, mImpl->mDescriptorSets );
    mImpl->mPipeline->Bind( inCommandBuffer );
}
