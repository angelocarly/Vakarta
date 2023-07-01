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

Mttr::Vox::VoxelRenderer::VoxelRenderer( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera )
{
    mVoxelPresenter = std::make_shared< Mttr::Vox::VoxelPresenter >( inDevice, inWidth, inHeight, inCamera, kWorldSize );
    mVoxelCompute = std::make_shared< Mttr::Vox::VoxelCompute >( inDevice, kWorldSize );
}

Mttr::Vox::VoxelRenderer::~VoxelRenderer()
{

}

int count = 0;
int totalcount = 0;

void
Mttr::Vox::VoxelRenderer::Draw( const vkrt::RenderEnvironment & inRenderEnvironment )
{
    auto theWorldBuffer = mVoxelPresenter->GetWorldBuffer();
    count++;
    totalcount++;
    if( count == 1 )
    {
        mVoxelCompute->Compute( inRenderEnvironment.mCommandBuffer, theWorldBuffer );
        count = 0;
    }
    mVoxelPresenter->Draw( inRenderEnvironment );
}
