/**
 * VoxelPresenter.cpp
 *
 * @file	VoxelPresenter.cpp
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#include "mttr/voxels/VoxelPresenter.h"

#include "vkrt/graphics/Presenter.h"

#include "vks/core/Buffer.h"
#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorLayoutBuilder.h"

#include <glm/vec3.hpp>

Mttr::Vox::VoxelPresenter::VoxelPresenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera, std::size_t inWorldSize )
:
    vkrt::Presenter( inDevice, inWidth, inHeight ),
    mDevice( inDevice ),
    mWidth( inWidth ),
    mHeight( inHeight ),
    mCamera( inCamera ),
    kWorldSize( inWorldSize )
{
    InitializeDescriptorSetLayout();
    InitializeDisplayPipeline( GetRenderPass() );
    InitializeBuffers();
}

Mttr::Vox::VoxelPresenter::~VoxelPresenter()
{
    mDevice->DestroyBuffer( mWorldBuffer );
    mDevice->GetVkDevice().destroy( mDescriptorSetLayout );
}

void
Mttr::Vox::VoxelPresenter::InitializeDescriptorSetLayout()
{
    mDescriptorSetLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eCompute )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
        .front();
}


void
Mttr::Vox::VoxelPresenter::InitializeDisplayPipeline( vk::RenderPass const inRenderPass )
{
    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/ScreenRect.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/Voxels.frag.spv" );

    std::vector< vk::PushConstantRange > thePushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eFragment,
            0,
            sizeof( PushConstants )
        )
    };

    vks::Pipeline::PipelineCreateInfo theCreateInfo =
    {
        inRenderPass,
        theVertexShader,
        theFragmentShader,
        { mDescriptorSetLayout },
        thePushConstants
    };

    vks::Pipeline::PipelineConfigInfo theConfigInfo =
    {
        vk::PrimitiveTopology::eTriangleList,
        {},
        {}
    };
    mPipeline = std::make_unique< vks::Pipeline >( mDevice, theCreateInfo, theConfigInfo );

    mDevice->GetVkDevice().destroy( theVertexShader );
    mDevice->GetVkDevice().destroy( theFragmentShader );
}

void
Mttr::Vox::VoxelPresenter::InitializeBuffers()
{
    auto theBufferSize = kWorldSize * kWorldSize * kWorldSize * sizeof( std::uint32_t );
    mWorldBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo( vk::BufferCreateFlags(), theBufferSize, vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc ),
        vma::AllocationCreateInfo( vma::AllocationCreateFlagBits::eHostAccessSequentialWrite, vma::MemoryUsage::eAuto )
    );
    auto theData = ( std::uint32_t * ) mDevice->MapMemory( mWorldBuffer );
    for( std::size_t i = 0; i < kWorldSize * kWorldSize * kWorldSize; ++i )
    {
//        if( rand() % 66000 == 0 )
//        {
//            theData[ i ] = rand() % 10;
//        }
//        else
        {
            theData[ i ] = 0;
        }
    }
    theData[ 0 ] = 1;
    mDevice->UnmapMemory( mWorldBuffer );
}

void
Mttr::Vox::VoxelPresenter::Draw( const vkrt::RenderEnvironment & inRenderEnvironment )
{
    auto theCommandBuffer = inRenderEnvironment.mCommandBuffer;

    BeginPresenterRenderPass( theCommandBuffer );
    {
        theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline->GetVkPipeline() );

        PushConstants thePushConstants
        {
            std::uint32_t( mWidth ),
            std::uint32_t( mHeight ),
            0,
            0,
            mCamera->GetModel(),
            mCamera->GetView(),
            mCamera->GetProjection()
        };
        theCommandBuffer.pushConstants
        (
            mPipeline->GetVkPipelineLayout(),
            vk::ShaderStageFlagBits::eFragment,
            0,
            sizeof( PushConstants ),
            & thePushConstants
        );

        auto theBufferInfo = vk::DescriptorBufferInfo
        (
            mWorldBuffer.GetVkBuffer(),
            0,
            mWorldBuffer.GetSize()
        );

        auto theWriteDescriptorSet = vk::WriteDescriptorSet();
        theWriteDescriptorSet.setDstBinding( 0 );
        theWriteDescriptorSet.setDstArrayElement( 0 );
        theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eStorageBuffer );
        theWriteDescriptorSet.setDescriptorCount( 1 );
        theWriteDescriptorSet.setPBufferInfo( & theBufferInfo );

        PFN_vkCmdPushDescriptorSetKHR pfnVkCmdPushDescriptorSetKhr = reinterpret_cast< PFN_vkCmdPushDescriptorSetKHR >( mDevice->GetVkDevice().getProcAddr( "vkCmdPushDescriptorSetKHR" ) );
        pfnVkCmdPushDescriptorSetKhr
        (
            theCommandBuffer,
            VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
            mPipeline->GetVkPipelineLayout(),
            0,
            1,
            reinterpret_cast< const VkWriteDescriptorSet * >(& theWriteDescriptorSet)
        );


        // Render
        theCommandBuffer.draw( 3, 1, 0, 0 );
    }
    EndPresenterRenderPass( theCommandBuffer );
}
