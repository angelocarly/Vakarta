/**
 * VoxelControls.cpp
 *
 * @file	VoxelControls.cpp
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#include "mttr/voxels/VoxelControls.h"

#include "imgui.h"

Mttr::Vox::VoxelControls::VoxelControls()
{

}

Mttr::Vox::VoxelControls::~VoxelControls()
{

}

void
Mttr::Vox::VoxelControls::DrawGui()
{
//    if (ImGui::BeginMainMenuBar())
//    {
//        if( ImGui::BeginMenu( "Menu" ) )
//        {
//            ImGui::EndMenu();
//        }
//        ImGui::EndMainMenuBar();
//    }

    ImGui::Begin( "Camera controls" );
    {
        ImGui::SliderFloat3( "Camera Position", &mCameraPosition.x, -1.0f, 1.0f );
    }
    ImGui::End();
}
