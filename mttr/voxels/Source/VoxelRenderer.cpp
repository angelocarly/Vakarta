/**
 * VoxelRenderer.cpp
 *
 * @file	VoxelRenderer.cpp
 * @author	Angelo Carly
 * @date	30/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "mttr/voxels/VoxelRenderer.h"

static const std::size_t kWorldSize = 128;

Vox::VoxelRenderer::VoxelRenderer( vks::DevicePtr inDevice, std::shared_ptr< VoxelControls > inVoxelControls, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera )
{
    mVoxelControls = inVoxelControls;
    mVoxelPresenter = std::make_shared< Vox::VoxelPresenter >( inDevice, inWidth, inHeight, inCamera, kWorldSize );
    mVoxelCompute = std::make_shared< Vox::VoxelCompute >( inDevice, kWorldSize, mVoxelControls );
    mParticleCompute = std::make_shared< Vox::ParticleCompute >( inDevice, kWorldSize, mVoxelControls );
}

Vox::VoxelRenderer::~VoxelRenderer()
{

}

int count = 0;
int totalcount = 0;

void
Vox::VoxelRenderer::Draw( const vkrt::RenderEnvironment & inRenderEnvironment )
{
    auto theWorldBuffer = mVoxelPresenter->GetWorldBuffer();
    count++;
    totalcount++;
    if( mVoxelControls->ShouldCompute() )
    {
        mVoxelCompute->Compute( inRenderEnvironment.mCommandBuffer, theWorldBuffer );
        count = 0;
    }
    mParticleCompute->Compute( inRenderEnvironment.mCommandBuffer, theWorldBuffer );

    mVoxelPresenter->Draw( inRenderEnvironment );
}
