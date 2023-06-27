#include "vkrt/application/Engine.h"

#include "vkrt/graphics/TestPresenter.h"
#include "vkrt/graphics/LayerPresenter.h"
#include "vkrt/gui/GuiPresenter.h"

#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

vkrt::Engine::Engine( vks::WindowPtr inWindow, vks::VulkanSessionPtr inVulkanSession, vkrt::PresenterPtr inPresenter )
:
    mWindow( inWindow ),
    mVulkanSession( inVulkanSession ),
    mRenderer( mVulkanSession, mWindow ),
    mInputState( std::make_shared< InputState >( mWindow ) ),
    mAssetLoader()
{
    auto theLayerPresenter = std::make_shared< vkrt::LayerPresenter >( mVulkanSession->GetDevice(), vk::Extent2D( WIDTH, HEIGHT ) );
    theLayerPresenter->AddPresenter( inPresenter );
    vkrt::GuiPresenter::Initialize( mVulkanSession->GetDevice(), mRenderer.GetSwapchain(), mWindow );
    theLayerPresenter->AddPresenter( vkrt::GuiPresenter::GetInstance() );
    mRenderer.SetPresenter( theLayerPresenter );
}

vkrt::Engine::~Engine()
{
    mVulkanSession->GetDevice()->GetVkDevice().waitIdle();
    vkrt::GuiPresenter::Finalize();
}

// ---------------------------------------------------------------------------------------------------------------------

void
vkrt::Engine::Update( float inFrameDuration )
{
    mWindow->Poll();
    mInputState->Sync();

    // Don't capture mouse if ImGui wants it

    ImGuiIO &io = ImGui::GetIO();
    if( io.WantCaptureMouse ) return;

    if( mInputState->IsButtonClicked( GLFW_MOUSE_BUTTON_1 ) )
    {
        mInputState->SwallowMouse();
    }
    if( mInputState->IsKeyDown( GLFW_KEY_ESCAPE ) )
    {
        mInputState->ReleaseMouse();
    }
}

void
vkrt::Engine::Render()
{
    mRenderer.Render();
}

bool
vkrt::Engine::ShouldClose()
{
    return mWindow->ShouldClose();
}

void
vkrt::Engine::RegisterGuiDrawer( std::weak_ptr< gui::GuiDrawer > inGuiDrawer )
{
    vkrt::GuiPresenter::GetInstance()->RegisterGuiDrawer( inGuiDrawer );
}

void
vkrt::Engine::Run()
{
    std::chrono::microseconds thePreviousFrameTime = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    std::chrono::microseconds theFrameTime;

    spdlog::info("Started game loop");
    std::size_t theFrameCount = 0;
    while( !ShouldClose() )
    {
        theFrameCount++;

        thePreviousFrameTime = theFrameTime;
        theFrameTime = std::chrono::duration_cast<std::chrono::microseconds>
        (
            std::chrono::system_clock::now().time_since_epoch()
        );
        float theFrameDuration = ( theFrameTime.count() - thePreviousFrameTime.count() ) % 10000000000 / 1000000.0f;

        Update( theFrameDuration );
        Render();

        float theSecondDuration = ( theFrameTime.count() - mPreviousSecond.count() ) % 10000000000 / 1000000.0f;
        if( theSecondDuration > 1 )
        {
            mPreviousSecond = theFrameTime;
            auto theFPS = theFrameCount;
            theFrameCount = 0;
            spdlog::info( "FPS: {}", theFPS );
        }
    }
}

