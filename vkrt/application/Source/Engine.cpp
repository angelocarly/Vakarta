#include "vkrt/application/Engine.h"

#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

vkrt::Engine::Engine()
:
    mWindow( std::make_shared< vks::Window >( WIDTH, HEIGHT, TITLE ) ),
    mVulkanSession( vks::VksSession::GetInstance() ),
    mRenderer( mVulkanSession, mWindow, { vkrt::RendererConfig::LINES } ),
    mInputState( std::make_shared< InputState >( mWindow ) ),
    mCamera( std::make_shared< vkrt::Camera >( 45, float( WIDTH ) / float( HEIGHT ), 0.1f, 250.0f ) ),
    mAssetLoader(),
    mReflectionPresenter( mVulkanSession ),
    mGuiLayer( mInputState, mRenderer.GetImGuiContext() )
{
    mCamera->SetPosition( glm::vec3( 0, 0, 5 ));
    mRenderer.SetCamera( mCamera );

    auto theMesh = mAssetLoader.LoadMeshResource( "resources/bunny.obj" );
    mMesh = std::make_unique< vks::Mesh >( mVulkanSession->GetDevice(), theMesh.GetVertices(), theMesh.GetIndices() );

}

vkrt::Engine::~Engine()
{
    mVulkanSession->GetDevice()->GetVkDevice().waitIdle();
}

// ---------------------------------------------------------------------------------------------------------------------

void
vkrt::Engine::Update( float inFrameDuration )
{
    // Render Gui
    mGuiLayer.Begin();
    mStats.mFps = 1.0f / inFrameDuration;
    mStats.mFrameTime = inFrameDuration;
//    vkrt::GuiPanels::Tools( mRenderer );
//    vkrt::GuiPanels::Stats( mStats );
    mGuiLayer.ImageTest( mReflectionPresenter );
    mGuiLayer.NodesTest( mReflectionPresenter );
    mGuiLayer.End();

    mWindow->Poll();

    ImGuiIO &io = ImGui::GetIO();
    if( io.WantCaptureMouse ) return;

    mInputState->Sync();

    if( mInputState->IsButtonClicked( GLFW_MOUSE_BUTTON_1 ) )
    {
        mInputState->SwallowMouse();
        mMouseSwallowed = true;
    }
    if( mInputState->IsKeyDown( GLFW_KEY_ESCAPE ) )
    {
        mInputState->ReleaseMouse();
        mMouseSwallowed = false;
    }

    if( mMouseSwallowed )
    {

        glm::vec2 theMouseDelta = mInputState->GetMouseDelta() / 1000.0f;
        mCamera->RotatePitch( theMouseDelta.y );
        mCamera->RotateYaw( theMouseDelta.x );

        // Switch render mode
        if( mInputState->IsKeyDown( GLFW_KEY_T ) )
        {
            auto theConfig = mRenderer.GetConfig();
            theConfig.topology = vkrt::RendererConfig::LINES;
            mRenderer.SetConfig( theConfig );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_Y ) )
        {
            auto theConfig = mRenderer.GetConfig();
            theConfig.topology = vkrt::RendererConfig::TRIANGLES;
            mRenderer.SetConfig( theConfig );
        }

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
            mCamera->Forward( 0.1f );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_S ) )
        {
            mCamera->Backward( 0.1f );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_D ) )
        {
            mCamera->Right( 0.1f );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_A ) )
        {
            mCamera->Left( 0.1f );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_SPACE ) )
        {
            mCamera->Up( 0.1f );
        }
        if( mInputState->IsKeyDown( GLFW_KEY_LEFT_SHIFT ) )
        {
            mCamera->Down( 0.1f );
        }
    }
}

void vkrt::Engine::Render()
{
    mRenderer.RenderFrame( * mMesh );
}

bool vkrt::Engine::ShouldClose()
{
    return mWindow->ShouldClose();
}

