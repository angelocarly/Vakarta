/**
 * Presenter.cpp
 *
 * @file	Presenter.cpp
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#include "mttr/scg/Presenter.h"

#include "vkrt/graphics/Presenter.h"

#include "vks/core/Buffer.h"
#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorLayoutBuilder.h"

#include <glm/vec3.hpp>

scg::Presenter::Presenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera )
:
    vkrt::Presenter( inDevice, inWidth, inHeight ),
    mDevice( inDevice ),
    mWidth( inWidth ),
    mHeight( inHeight ),
    mCamera( inCamera )
{
    InitializeDescriptorSetLayout();
    InitializeDisplayPipeline( GetRenderPass() );
    InitializeBuffers();
}

scg::Presenter::~Presenter()
{
    mDevice->UnmapMemory( mGeometryBuffer );
    mDevice->GetVkDevice().destroy( mDescriptorSetLayout );
    mDevice->DestroyBuffer( mGeometryBuffer );
}

void
scg::Presenter::InitializeDescriptorSetLayout()
{
    mDescriptorSetLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eStorageBuffer, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eCompute )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
        .front();
}


void
scg::Presenter::InitializeDisplayPipeline( vk::RenderPass const inRenderPass )
{
    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/ScreenRect.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/scg.frag.spv" );

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
scg::Presenter::InitializeBuffers()
{
    auto theBufferSize = sizeof( std::uint32_t ) * 100;
    mGeometryBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo( vk::BufferCreateFlags(), theBufferSize, vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc ),
        vma::AllocationCreateInfo( vma::AllocationCreateFlagBits::eHostAccessSequentialWrite, vma::MemoryUsage::eAuto )
    );

    mSymBuffer = ( std::uint32_t * ) mDevice->MapMemory( mGeometryBuffer );
    for( int i = 0; i < 100; ++i )
    {
        mSymBuffer[i] = 0;
    }
}

void
scg::Presenter::Draw( const vkrt::RenderEnvironment & inRenderEnvironment )
{
    auto theCommandBuffer = inRenderEnvironment.mCommandBuffer;

    BeginPresenterRenderPass( theCommandBuffer );
    {
        theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline->GetVkPipeline() );

        theCommandBuffer.pushConstants
        (
            mPipeline->GetVkPipelineLayout(),
            vk::ShaderStageFlagBits::eFragment,
            0,
            sizeof( PushConstants ),
            & mPushConstants
        );

        auto theBufferInfo = vk::DescriptorBufferInfo
        (
            mGeometryBuffer.GetVkBuffer(),
            0,
            VK_WHOLE_SIZE
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
