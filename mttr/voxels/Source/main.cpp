#include <iostream>
#include <cstdlib>
#include <spdlog/spdlog.h>

#include "vkrt/application/Engine.h"
#include "mttr/voxels/VoxelPresenter.h"
#include "mttr/voxels/VoxelControls.h"

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

int main()
{
    auto theWindow = std::make_shared< vks::Window >( WIDTH, HEIGHT, TITLE );
    auto theVulkanSession = vks::VksSession::GetInstance();

    auto theVoxelPresenter = std::make_shared< Mttr::Vox::VoxelPresenter >( theVulkanSession->GetDevice(), WIDTH, HEIGHT );

    auto engine = std::make_shared< vkrt::Engine >( theWindow, theVulkanSession, theVoxelPresenter );

    auto theVoxelController = std::make_shared< Mttr::Vox::VoxelControls >( engine );
    theVoxelPresenter->SetVoxelControls( theVoxelController );
    engine->RegisterGuiDrawer( theVoxelController );

    engine->Run();

    return EXIT_SUCCESS;
}
