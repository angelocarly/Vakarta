/**
 * VoxelControls.cpp
 *
 * @file	VoxelControls.cpp
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#include "mttr/voxels/VoxelControls.h"

#include "vkrt/core/Camera.h"

#include "imgui.h"

Mttr::Vox::VoxelControls::VoxelControls( vkrt::InputStatePtr inInputState )
:
    mInputState( inInputState ),
    mCamera( std::make_shared< vkrt::Camera >( 90.0f, 900.0f / 1600.0f, .01f, 1000.0f ) )
{
    mCamera->SetPosition( glm::vec3( 0.0f, 0.0f, -5.0f ) );
}

Mttr::Vox::VoxelControls::~VoxelControls()
{

}

void
Mttr::Vox::VoxelControls::DrawGui()
{
    ImGuiIO &io = ImGui::GetIO();

    if( mInputState->IsButtonClicked( GLFW_MOUSE_BUTTON_1 ) )
    {
        mInputState->SwallowMouse();
    }
    if( mInputState->IsKeyDown( GLFW_KEY_ESCAPE ) )
    {
        mInputState->ReleaseMouse();
    }

    if( mInputState->IsMouseSwallowed() )
    {

        glm::vec2 theMouseDelta = mInputState->GetMouseDelta() / 1000.0f;
        mCamera->RotatePitch( theMouseDelta.y );
        mCamera->RotateYaw( theMouseDelta.x );

        float speed = 1.1f;

        if( mInputState->IsKeyDown( GLFW_KEY_Q ) )
        {
            mCamera->RotateRoll( -0.1f );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_E ) )
        {
            mCamera->RotateRoll( 0.1f );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_W ) )
        {
            mCamera->Forward( speed );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_S ) )
        {
            mCamera->Backward( speed );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_D ) )
        {
            mCamera->Right( speed );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_A ) )
        {
            mCamera->Left( speed );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_SPACE ) )
        {
            mCamera->Up( speed );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_LEFT_SHIFT ) )
        {
            mCamera->Down( speed );
        }
    }

    ImGui::Begin( "Simulation controls" );
    {
        ImGui::Checkbox( "Compute", & mComputeEnabled );
        ImGui::SliderInt( "Frames per compute", reinterpret_cast< int * >( & mFramesPerCompute ), 1, 100 );
        mStepCompute = ImGui::Button( "Step simulation");
    }
    ImGui::End();

    mShouldCompute = false;
    mFrameCount++;
    if( mFrameCount >= mFramesPerCompute )
    {
        mFrameCount = 0;
        mShouldCompute = mComputeEnabled;
    }
    if( mStepCompute ) mShouldCompute = true;

}

glm::mat4
Mttr::Vox::VoxelControls::GetModel() const
{
    return mCamera->GetModel();
}

glm::mat4
Mttr::Vox::VoxelControls::GetView() const
{
    return mCamera->GetView();
}

glm::mat4
Mttr::Vox::VoxelControls::GetProjection() const
{
    return mCamera->GetProjection();
}
