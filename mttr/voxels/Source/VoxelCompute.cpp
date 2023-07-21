/**
 * VoxelCompute.cpp
 *
 * @file	VoxelCompute.cpp
 * @author	Angelo Carly
 * @date	27/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "mttr/voxels/VoxelCompute.h"

#include "vks/render/ComputePipeline.h"
#include "vks/render/DescriptorLayoutBuilder.h"
#include "vks/render/Device.h"
#include "vks/render/Utils.h"

#include <chrono>

Mttr::Vox::VoxelCompute::VoxelCompute( vks::DevicePtr inDevice, std::size_t inWorldSize, std::shared_ptr< Mttr::Vox::VoxelControls > inControls )
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

Mttr::Vox::VoxelCompute::~VoxelCompute()
{
    mDevice->DestroyBuffer( mReadWorldBuffer );
    mDevice->GetVkDevice().destroy( mDescriptorSetLayout );
}

void
Mttr::Vox::VoxelCompute::InitializeDescriptorSetLayout()
{
    mDescriptorSetLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eCompute )
        .AddLayoutBinding( 1, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eCompute )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
        .front();
}

void
Mttr::Vox::VoxelCompute::InitializeComputePipeline()
{
    auto theComputeShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/Voxels.comp.spv" );

    std::vector< vk::PushConstantRange > thePushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eCompute,
            0,
            sizeof( PushConstants )
        )
    };


    vks::ComputePipeline::ComputePipelineCreateInfo theComputePipelineCreateInfo =
    {
        theComputeShader,
        { mDescriptorSetLayout },
        thePushConstants
    };
    mComputePipeline = std::make_unique< vks::ComputePipeline >( mDevice, theComputePipelineCreateInfo );

    mDevice->GetVkDevice().destroy( theComputeShader );
}

void
Mttr::Vox::VoxelCompute::InitializeBuffers()
{
    auto theBufferSize = kWorldSize * kWorldSize * kWorldSize * sizeof( std::uint32_t );
    mReadWorldBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo( vk::BufferCreateFlags(), theBufferSize, vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst ),
        vma::AllocationCreateInfo( vma::AllocationCreateFlagBits::eHostAccessSequentialWrite, vma::MemoryUsage::eAuto )
    );
}

void
Mttr::Vox::VoxelCompute::Compute( vk::CommandBuffer const inCommandBuffer, vks::Buffer & inWorldBuffer )
{
    assert( inWorldBuffer.GetSize() == kWorldSize * kWorldSize * kWorldSize * sizeof( std::uint32_t ) );

    auto theCommandBuffer = inCommandBuffer;

    theCommandBuffer.copyBuffer
    (
        inWorldBuffer.GetVkBuffer(),
        mReadWorldBuffer.GetVkBuffer(),
        vk::BufferCopy( 0, 0, inWorldBuffer.GetSize() )
    );

    mComputePipeline->Bind( theCommandBuffer );

    auto theBufferInfo = vk::DescriptorBufferInfo
    (
        inWorldBuffer.GetVkBuffer(),
        0,
        inWorldBuffer.GetSize()
    );
    auto theReadBufferInfo = vk::DescriptorBufferInfo
    (
        mReadWorldBuffer.GetVkBuffer(),
        0,
        mReadWorldBuffer.GetSize()
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
