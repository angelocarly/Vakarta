/**
 * VoxelEngine.cpp
 *
 * @file	VoxelEngine.cpp
 * @author	Angelo Carly
 * @date	29/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "mttr/voxels/VoxelEngine.h"

#include <iostream>
#include <cstdlib>
#include <spdlog/spdlog.h>

#include "vkrt/application/Engine.h"
#include "mttr/voxels/VoxelPresenter.h"
#include "mttr/voxels/VoxelControls.h"
#include "mttr/voxels/VoxelRenderer.h"

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

Mttr::Vox::VoxelEngine::VoxelEngine( vks::WindowPtr inWindow, vks::VksSessionPtr inSession )
:
    vkrt::Engine( inWindow, inSession )
{
    mVoxelGui = std::make_shared< Mttr::Vox::VoxelControls >( GetInputState() );
    RegisterGuiDrawer( mVoxelGui );

    mVoxelRenderer = std::make_shared< Mttr::Vox::VoxelRenderer >( inSession->GetDevice(), WIDTH, HEIGHT, mVoxelGui->GetCamera() );
    RegisterPresenter( mVoxelRenderer );
}

Mttr::Vox::VoxelEngine::~VoxelEngine()
{

}

void
Mttr::Vox::VoxelEngine::Update( float inFrameDuration )
{

}

// ========================================= Main entrypoint ===========================================================

int main()
{
    auto theWindow = std::make_shared< vks::Window >( WIDTH, HEIGHT, TITLE );
    auto theVulkanSession = vks::VksSession::GetInstance();

    std::shared_ptr< vkrt::Engine > engine = std::make_shared< Mttr::Vox::VoxelEngine >( theWindow, theVulkanSession );

    engine->Run();

    engine.reset();
    theVulkanSession.reset();

    return EXIT_SUCCESS;
}
