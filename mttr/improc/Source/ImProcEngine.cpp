/**
 * ImprocEngine.cpp
 *
 * @file	ImprocEngine.cpp
 * @author	Angelo Carly
 * @date	29/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "mttr/improc/ImProcEngine.h"

#include <iostream>
#include <cstdlib>
#include <spdlog/spdlog.h>

#include "vkrt/application/Engine.h"

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

Mttr::Imp::ImProcEngine::ImProcEngine( vks::WindowPtr inWindow, vks::VksSessionPtr inSession )
:
    vkrt::Engine( inWindow, inSession )
{
    mCamera = std::make_shared< vkrt::Camera >( 90.0f, WIDTH / HEIGHT, .1f, 1000.0f );

    mPresenter = std::make_shared< Mttr::Imp::Presenter >( inSession->GetDevice(), WIDTH, HEIGHT, mCamera );
    RegisterPresenter( mPresenter );

    mGui = std::make_shared< Mttr::Imp::ImpGUI >( GetInputState(), mPresenter );
    RegisterGuiDrawer( mGui );


}

Mttr::Imp::ImProcEngine::~ImProcEngine()
{

}

void
Mttr::Imp::ImProcEngine::Update( float inFrameDuration )
{

}

// ========================================= Main entrypoint ===========================================================

int main()
{
    auto theWindow = std::make_shared< vks::Window >( WIDTH, HEIGHT, TITLE );
    auto theVulkanSession = vks::VksSession::GetInstance();

    std::shared_ptr< vkrt::Engine > engine = std::make_shared< Mttr::Imp::ImProcEngine >( theWindow, theVulkanSession );

    engine->Run();

    engine.reset();
    theVulkanSession.reset();

    return EXIT_SUCCESS;
}
