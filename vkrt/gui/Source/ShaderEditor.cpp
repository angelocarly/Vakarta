//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/ShaderEditor.h"

#include <memory>
#include <imgui.h>
#include <zep/editor.h>
#include <zep/imgui/editor_imgui.h>

struct vkrt::gui::ShaderEditor::ZepWrapper : public Zep::IZepComponent
{
    ZepWrapper()
    :
        mEditor( Zep::fs::path( "." ), Zep::NVec2f( 1, 1) )
    {

    }

    virtual Zep::ZepEditor& GetEditor() const override
    {
        return ( Zep::ZepEditor& ) mEditor;
    }

    Zep::ZepEditor_ImGui mEditor;
    std::function<void(Zep::ZepMessage)> Callback;
};

vkrt::gui::ShaderEditor::ShaderEditor()
{
    zepWrapper = std::make_shared< vkrt::gui::ShaderEditor::ZepWrapper >();
//    zepWrapper->mEditor.InitWithFileOrDir( "./test.md");
    zepWrapper->mEditor.InitWithText( "Hello World", "Hello world" );

    // TODO: Add custom theme
//    auto theTheme = std::make_shared<Zep::ZepTheme>();
//    zepWrapper->GetEditor().GetActiveBuffer()->SetTheme( theTheme );

    auto& display = zepWrapper->mEditor.GetEditor().GetDisplay();
    auto pImFont = ImGui::GetIO().Fonts[0].Fonts[0];
    auto pixelHeight = pImFont->FontSize;
    display.SetFont(Zep::ZepTextType::UI, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight)));
    display.SetFont(Zep::ZepTextType::Text, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight)));
    display.SetFont(Zep::ZepTextType::Heading1, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight * 1.5)));
    display.SetFont(Zep::ZepTextType::Heading2, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight * 1.25)));
    display.SetFont(Zep::ZepTextType::Heading3, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight * 1.125)));
}

vkrt::gui::ShaderEditor::~ShaderEditor()
{

}

void
vkrt::gui::ShaderEditor::Draw( bool & inOpen )
{
    if( !inOpen ) return;

    ImGui::SetNextWindowSize(ImVec2( 200, 200 ), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Zep", & inOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    auto min = ImGui::GetCursorScreenPos();
    auto max = ImGui::GetContentRegionAvail();
    if (max.x <= 0)
        max.x = 1;
    if (max.y <= 0)
        max.y = 1;
    ImGui::InvisibleButton("ZepContainer", max);

    // Fill the window
    max.x = min.x + max.x;
    max.y = min.y + max.y;

    zepWrapper->GetEditor().SetDisplayRegion(Zep::NVec2f(min.x, min.y), Zep::NVec2f(max.x, max.y));
    zepWrapper->GetEditor().Display();
    bool zep_focused = ImGui::IsWindowFocused();
    if (zep_focused)
    {
        zepWrapper->mEditor.HandleInput();
    }

    ImGui::End();
}
