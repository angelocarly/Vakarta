/**
 * VoxelPresenter.cpp
 *
 * @file	VoxelPresenter.cpp
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#include "mttr/voxels/VoxelPresenter.h"

#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"

#include <glm/vec3.hpp>

Mttr::Vox::VoxelPresenter::VoxelPresenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight )
:
    vkrt::Presenter( inDevice, inWidth, inHeight ),
    mDevice( inDevice ),
    mWidth( inWidth ),
    mHeight( inHeight )
{
    InitializePipeline( GetRenderPass() );
}

Mttr::Vox::VoxelPresenter::~VoxelPresenter()
{

}

void
Mttr::Vox::VoxelPresenter::InitializePipeline( vk::RenderPass const inRenderPass )
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
        {},
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
Mttr::Vox::VoxelPresenter::SetVoxelControls( std::shared_ptr< Mttr::Vox::VoxelControls > inVoxelControls )
{
    mVoxelControls = inVoxelControls;
}

void
Mttr::Vox::VoxelPresenter::Draw( const vkrt::RenderEnvironment & inRenderEnvironment )
{
    auto theCommandBuffer = inRenderEnvironment.mCommandBuffer;

    BeginPresenterRenderPass( theCommandBuffer );
    {
        theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline->GetVkPipeline() );

        if( !mVoxelControls )
        {
            throw std::runtime_error( "VoxelPresenter::Draw() - VoxelControls not set" );
        }
        PushConstants thePushConstants
        {
            std::uint32_t( mWidth ),
            std::uint32_t( mHeight ),
            0,
            0,
            glm::vec4( mVoxelControls->GetCameraPosition(), 0.0f )
        };
        theCommandBuffer.pushConstants
        (
            mPipeline->GetVkPipelineLayout(),
            vk::ShaderStageFlagBits::eFragment,
            0,
            sizeof( PushConstants ),
            & thePushConstants
        );

        // Render
        theCommandBuffer.draw( 3, 1, 0, 0 );
    }
    EndPresenterRenderPass( theCommandBuffer );
}
