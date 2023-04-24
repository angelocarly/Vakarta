//
// Created by magnias on 1/29/23.
//

#include "vkrt/gui/GuiLayer.h"

#include "vkrt/application/Engine.h"
#include "imnodes/imnodes.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

#define ZEP_SINGLE_HEADER_BUILD

#include <zep.h>

vkrt::GuiLayer::GuiLayer( vkrt::InputStatePtr inInputState, ImGuiContext * inContext )
{
    ImGui::SetCurrentContext( inContext );
    mGuiNodes = std::make_shared< GuiNodes >( inInputState );
}

vkrt::GuiLayer::~GuiLayer()
{

}

void zep_init()
{
}

void
vkrt::GuiLayer::Begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = true;

    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport();

    if( mShowDemoWindow )
    {
        ImGui::ShowDemoWindow();
    }

    if (ImGui::BeginMainMenuBar())
    {
        if( ImGui::BeginMenu( "Menu" ) )
        {
            ImGui::MenuItem("Show stats" , nullptr, &mShowStats );
            ImGui::MenuItem("Show tools" , nullptr, &mShowTools );
            ImGui::MenuItem("Show demo window" , nullptr, &mShowDemoWindow );
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}

void
vkrt::GuiLayer::Stats( vkrt::GuiLayer::StatsInfo & inStats )
{
    if( !mShowStats ) return;

    // Style
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoMove;

    // Position
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 window_pos, window_pos_pivot;
    window_pos = work_pos;
    window_pos.x += 10.0f;
    window_pos.y += 10.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

    // Stats
    bool pOpen = false;
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    ImGui::Begin("Overlay", &pOpen, window_flags );
    {
        ImGui::Text("Stats" );
        ImGui::Separator();
        ImGui::Text("frametime: %.2f ms", inStats.mFrameTime * 1000.0f );
        ImGui::Text("fps: %.1f (frames/second)", inStats.mFps );
    }
    ImGui::End();
}

void
vkrt::GuiLayer::Tools( vkrt::Renderer & inRenderer )
{
    if( !mShowTools ) return;

    // Stats
//    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    bool pOpen = false;
    ImGuiWindowFlags window_flags = 0;
    ImGui::Begin("Tools", &pOpen, window_flags );
    {
        if( ImGui::Button( "Reset mesh" ) )
        {
            inRenderer.Recompute( true );
        }

        static bool pCompute = false;
        ImGui::Checkbox( "Continuous compute", & pCompute );
        if( pCompute )
        {
            inRenderer.Recompute( false );
        }
    }
    ImGui::End();
}

void
vkrt::GuiLayer::ImageTest( vkrt::Presenter & inPresenter )
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::Begin("ImageTest" );
    {
        ImVec2 wsize = ImGui::GetContentRegionMax();
        wsize.y -= 20;
        ImGui::Image( inPresenter.GetImage(), wsize );
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void
vkrt::GuiLayer::NodesTest( vkrt::Presenter & inPresenter )
{
    mGuiNodes->Draw( inPresenter );
}

void
vkrt::GuiLayer::End()
{
    // Rendering
    ImGui::Render();
}
