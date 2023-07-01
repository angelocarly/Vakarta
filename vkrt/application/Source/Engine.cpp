#include "vkrt/application/Engine.h"

#include "vkrt/graphics/TestPresenter.h"
#include "vkrt/graphics/LayerPresenter.h"
#include "vkrt/gui/GuiPresenter.h"

#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

vkrt::Engine::Engine( vks::WindowPtr inWindow, vks::VksSessionPtr inVulkanSession )
:
    mWindow( inWindow ),
    mVulkanSession( inVulkanSession ),
    mRenderer( mVulkanSession, mWindow ),
    mInputState( std::make_shared< InputState >( mWindow ) ),
    mAssetLoader()
{
    vkrt::GuiPresenter::Initialize( mVulkanSession->GetDevice(), mRenderer.GetSwapchain(), mWindow );

    mPreviousSecond = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
}

vkrt::Engine::~Engine()
{
    mVulkanSession->GetDevice()->GetVkDevice().waitIdle();
    vkrt::GuiPresenter::Finalize();
}

// ---------------------------------------------------------------------------------------------------------------------

void
vkrt::Engine::UpdateSelf( float inFrameDuration )
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
vkrt::Engine::RenderSelf()
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
vkrt::Engine::RegisterPresenter( std::shared_ptr< Presentable > inPresenter )
{
    auto theLayerPresenter = std::make_shared< vkrt::LayerPresenter >( mVulkanSession->GetDevice(), vk::Extent2D( WIDTH, HEIGHT ) );
    theLayerPresenter->AddPresenter( inPresenter );
    theLayerPresenter->AddPresenter( vkrt::GuiPresenter::GetInstance() );
    mRenderer.SetPresenter( theLayerPresenter );
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
        UpdateSelf( theFrameDuration );
        RenderSelf();

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

void
vkrt::Engine::AddDrawable( std::shared_ptr< Drawable > inDrawable )
{
    mDrawables.push_back( inDrawable );
}

