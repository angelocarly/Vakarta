//
// Created by Angelo Carly on 18/12/2022.
//

#include "vkrt/graphics/GeoGenPipeline.h"

#include "vks/core/Vertex.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorLayoutBuilder.h"
#include "vks/render/DescriptorSet.h"
#include "vks/render/ComputePipeline.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vulkan/vulkan.hpp>

namespace
{
    struct PushConstantUniformObject
    {
        int mInvocationCount;
    };
};

class vkrt::GeoGenPipeline::Impl
{
    public:
        Impl( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass );
        ~Impl();

    private:
        void InitializeDescriptors();
        void InitializePipeline();

    public:
        vks::DevicePtr mDevice;
        vks::RenderPassPtr mRenderPass;
        vks::ComputePipelinePtr mPipeline;

        // Descriptors
        vk::DescriptorPool mDescriptorPool;
        std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
        std::vector< vks::DescriptorSet > mDescriptorSets;
        vks::Buffer mOutputBuffer;
};

vkrt::GeoGenPipeline::Impl::Impl( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass )
:
    mDevice( inDevice ),
    mRenderPass( inRenderPass )
{
    CreateBuffers();
    InitializeDescriptors();
    InitializePipeline();
}

vkrt::GeoGenPipeline::Impl::~Impl()
{
    mDevice->DestroyBuffer( mOutputBuffer );

    for( auto theDescriptorSetLayout : mDescriptorSetLayouts )
    {
        mDevice->GetVkDevice().destroy( theDescriptorSetLayout );
    }
    mDevice->GetVkDevice().destroy( mDescriptorPool );
}

void
vkrt::GeoGenPipeline::Impl::InitializeDescriptors()
{
    // Pool
    std::array< vk::DescriptorPoolSize, 1 > thePoolSizes = {};
    thePoolSizes[ 0 ].setType( vk::DescriptorType::eStorageBuffer );
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
    mDescriptorSetLayouts = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eCompute )
        .Build( mDevice );

    // Descriptor set
    auto theDescriptorSet = vks::DescriptorSet( mDevice, mDescriptorPool, mDescriptorSetLayouts.front() );
    mDescriptorSets = { theDescriptorSet };
}

void
vkrt::GeoGenPipeline::Impl::InitializePipeline()
{
    auto theComputeShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/GeoGen.comp.spv" );

    std::vector< vk::PushConstantRange > thePushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eCompute,
            0,
            sizeof( PushConstantUniformObject )
        )
    };

    vks::ComputePipeline::ComputePipelineCreateInfo theCreateInfo =
    {
        mRenderPass->GetVkRenderPass(),
        theComputeShader,
        mDescriptorSetLayouts,
        thePushConstants
    };
    mPipeline = std::make_unique< vks::ComputePipeline >( mDevice, theCreateInfo );

    mDevice->GetVkDevice().destroy( theComputeShader );
}

// =====================================================================================================================

vkrt::GeoGenPipeline::GeoGenPipeline( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass )
:
    mImpl( new Impl( inDevice, inRenderPass ) )
{

}

vkrt::GeoGenPipeline::~GeoGenPipeline()
{

}

void
vkrt::GeoGenPipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    mImpl->mPipeline->BindDescriptorSets( inCommandBuffer, mImpl->mDescriptorSets );
    mImpl->mPipeline->Bind( inCommandBuffer );
}

void
vkrt::GeoGenPipeline::Dispatch( vk::CommandBuffer inCommandBuffer, vks::Buffer inOutputBuffer, std::uint32_t inVertexCount )
{
    // Bind output buffer
    mImpl->mDescriptorSets.at( 0 ).BindBuffer( 0, vk::DescriptorType::eStorageBuffer, inOutputBuffer );

    PushConstantUniformObject thePushConstants;
    thePushConstants.mInvocationCount = inVertexCount;
    mImpl->mPipeline->PushConstants( inCommandBuffer, sizeof( PushConstantUniformObject ), &thePushConstants );
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eCompute, mImpl->mPipeline->GetVkPipeline(), {} );
    inCommandBuffer.dispatch( 16, 1, 1 );
}
