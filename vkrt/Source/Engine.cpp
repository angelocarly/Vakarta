#include "vkrt/Engine.h"
#include "imgui/imgui.h"

#include <spdlog/spdlog.h>

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

Engine::Engine()
:
    mWindow( std::make_shared< vks::Window >( WIDTH, HEIGHT, TITLE ) ),
    mRenderer( mWindow ),
    mInputState( mWindow ),
    mCamera( std::make_shared< vkrt::Camera >( 45, float( WIDTH ) / float( HEIGHT ), 0.1f, 100.0f ) )
{
    mCamera->SetPosition( glm::vec3( 0, 0, 5 ));
    mRenderer.SetCamera( mCamera );
}

Engine::~Engine()
{
}

// -------------------------------------------------------------------------------------------------------------------

void Engine::Update()
{
    mWindow->Poll();

    ImGuiIO &io = ImGui::GetIO();
    if( io.WantCaptureMouse ) return;

    mInputState.Sync();

    if( mInputState.IsButtonClicked( GLFW_MOUSE_BUTTON_1 ) )
    {
        mInputState.SwallowMouse();
        mMouseSwallowed = true;
    }
    if( mInputState.IsKeyDown( GLFW_KEY_ESCAPE ) )
    {
        mInputState.ReleaseMouse();
        mMouseSwallowed = false;
    }

    if( mMouseSwallowed )
    {

        glm::vec2 theMouseDelta = mInputState.GetMouseDelta() / 1000.0f;
        mCamera->RotatePitch( theMouseDelta.y );
        mCamera->RotateYaw( theMouseDelta.x );

        if( mInputState.IsKeyDown( GLFW_KEY_W ) )
        {
            mCamera->Forward( 0.1f );
        }
        if( mInputState.IsKeyDown( GLFW_KEY_S ) )
        {
            mCamera->Backward( 0.1f );
        }
        if( mInputState.IsKeyDown( GLFW_KEY_D ) )
        {
            mCamera->Right( 0.1f );
        }
        if( mInputState.IsKeyDown( GLFW_KEY_A ) )
        {
            mCamera->Left( 0.1f );
        }
        if( mInputState.IsKeyDown( GLFW_KEY_SPACE ) )
        {
            mCamera->Up( 0.1f );
        }
        if( mInputState.IsKeyDown( GLFW_KEY_LEFT_SHIFT ) )
        {
            mCamera->Down( 0.1f );
        }
    }
}

void Engine::Render()
{
    mRenderer.Render();
}

bool Engine::ShouldClose()
{
    return mWindow->ShouldClose();
}

