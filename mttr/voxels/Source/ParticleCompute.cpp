/**
 * VoxelCompute.cpp
 *
 * @file	ParticleCompute.cpp
 * @author	Angelo Carly
 * @date	29/07/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "mttr/voxels/VoxelCompute.h"

#include "vks/render/ComputePipeline.h"
#include "vks/render/DescriptorLayoutBuilder.h"
#include "vks/render/Device.h"
#include "vks/render/Utils.h"
#include "mttr/voxels/ParticleCompute.h"

#include <chrono>

Vox::ParticleCompute::ParticleCompute( vks::DevicePtr inDevice, std::size_t inWorldSize, std::shared_ptr< Vox::VoxelControls > inControls )
:
    mDevice( inDevice ),
    mControls( inControls ),
    kWorldSize( inWorldSize )
{
    InitializeDescriptorSetLayout();
    InitializeComputePipeline();
    InitializeBuffers();

    mStartTime = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
}

Vox::ParticleCompute::~ParticleCompute()
{
    mDevice->UnmapMemory( mParticleBuffer );
    mDevice->DestroyBuffer( mParticleBuffer );
    mDevice->GetVkDevice().destroy( mDescriptorSetLayout );
}

void
Vox::ParticleCompute::InitializeDescriptorSetLayout()
{
    mDescriptorSetLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eCompute )
        .AddLayoutBinding( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eCompute )
        .AddLayoutBinding( 2, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eCompute )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
        .front();
}

void
Vox::ParticleCompute::InitializeComputePipeline()
{
    auto theComputeShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/particles.comp.spv" );

    std::vector< vk::PushConstantRange > thePushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eCompute,
            0,
            sizeof( PushConstants )
        )
    };

    std::array< vk::SpecializationMapEntry, 4 > theMapEntries =
    {
        vk::SpecializationMapEntry{ 0, 0, sizeof( int ) },
        vk::SpecializationMapEntry{ 1, 0, sizeof( int ) },
        vk::SpecializationMapEntry{ 2, 0, sizeof( int ) },
        vk::SpecializationMapEntry{ 4, 0, sizeof( int ) }
    };
    struct SpecializationData
    {
        int mResolutionX;
        int mResolutionY;
        int mWorldSize;
    };
    SpecializationData theSpecializationData =
    {
        1024,
        1024,
        10
    };

    vk::SpecializationInfo theSpecializationInfo =
    {
        theMapEntries.size(),
        theMapEntries.data(),
        sizeof( int ) * 4,
        & theSpecializationData
    };

    mComputePipeline = vks::ComputePipelineBuilder( mDevice )
        .SetComputeShader( theComputeShader)
        .SetPushConstants( thePushConstants )
        .SetDescriptorSetLayouts( { mDescriptorSetLayout } )
        .SetSpecializationInfo( theSpecializationInfo )
        .Build();

    mDevice->GetVkDevice().destroy( theComputeShader );
}

void
Vox::ParticleCompute::InitializeBuffers()
{
    auto theBufferSize = kParticleCount * sizeof( glm::vec4 );
    mParticleBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo( vk::BufferCreateFlags(), theBufferSize, vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst ),
        vma::AllocationCreateInfo( vma::AllocationCreateFlagBits::eHostAccessSequentialWrite, vma::MemoryUsage::eAuto )
    );

    glm::vec4 * theData = ( glm::vec4 * ) mDevice->MapMemory(  mParticleBuffer );
    for( std::size_t i = 0; i < kParticleCount; ++i )
    {
        auto theParticle = glm::vec4
        (
            rand() % kWorldSize,
            rand() % kWorldSize,
            rand() % kWorldSize,
            1.0f
        );
        theData[ i ] = theParticle;

    }
}

void
Vox::ParticleCompute::Compute( vk::CommandBuffer const inCommandBuffer, vks::Buffer & inWorldBuffer )
{
    assert( inWorldBuffer.GetSize() == kWorldSize * kWorldSize * kWorldSize * sizeof( glm::vec4 ) );

    auto theCommandBuffer = inCommandBuffer;

    mComputePipeline->Bind( theCommandBuffer );

    auto theBufferInfo = vk::DescriptorBufferInfo
    (
        inWorldBuffer.GetVkBuffer(),
        0,
        inWorldBuffer.GetSize()
    );
    auto theReadBufferInfo = vk::DescriptorBufferInfo
    (
        mParticleBuffer.GetVkBuffer(),
        0,
        mParticleBuffer.GetSize()
    );

    auto theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstBinding( 0 );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eStorageBuffer );
    theWriteDescriptorSet.setDescriptorCount( 1 );
    theWriteDescriptorSet.setPBufferInfo( & theReadBufferInfo );
    mComputePipeline->PushDescriptor( theCommandBuffer, theWriteDescriptorSet );

    theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstBinding( 1 );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eStorageBuffer );
    theWriteDescriptorSet.setDescriptorCount( 1 );
    theWriteDescriptorSet.setPBufferInfo( & theBufferInfo );
    mComputePipeline->PushDescriptor( theCommandBuffer, theWriteDescriptorSet );

    auto theTime = std::chrono::duration_cast<std::chrono::microseconds>
        (
            std::chrono::system_clock::now().time_since_epoch()
        );
    PushConstants thePushConstants
        {
            ( float ) ( mStartTime - theTime ).count() / 1000.0f
        };
    theCommandBuffer.pushConstants
        (
            mComputePipeline->GetVkPipelineLayout(),
            vk::ShaderStageFlagBits::eCompute,
            0,
            sizeof( PushConstants ),
            & thePushConstants
        );


    int theGroupSize = 8;
    int theGroupCount = kWorldSize / theGroupSize;
    theCommandBuffer.dispatch( theGroupCount, theGroupCount, theGroupCount );

}
