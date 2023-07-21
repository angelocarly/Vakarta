//
// Created by magnias on 7/21/23.
//

#include "mttr/improc/ImpGUI.h"

#include "vks/assets/AssetLoader.h"
#include "vks/render/Device.h"
#include "vks/render/VksSession.h"


#include <ImGuiFileDialog.h>

Mttr::Imp::ImpGUI::ImpGUI( vkrt::InputStatePtr inInputState, std::shared_ptr< Mttr::Imp::Presenter > inPresenter )
:
    GuiDrawer(),
    mPresenter( inPresenter ),
    mInputState( inInputState )
{

}

void
Mttr::Imp::ImpGUI::DrawGui()
{
    ImGui::Begin( "Image settings" );
    {

        if( ImGui::Button( "Load PNG.." ) )
        {
            ImGuiFileDialog::Instance()->OpenDialog( "Id", "Choose File", ".png", "." );
        }

        ImGui::ColorPicker3( "Color treshold", &mPresenter->GetColorTreshold()[ 0 ] );
        ImGui::ColorPicker3( "Color replacement", &mPresenter->GetReplacementColor()[ 0 ] );

    }
    ImGui::End();


    if( ImGuiFileDialog::Instance()->Display( "Id" ) )
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            mFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
            mFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
            // action
            auto theImageResource = vks::AssetLoader::LoadImageResource( mFilePath );
            auto theDevice = vks::VksSession::GetInstance()->GetDevice();
            mImage = theDevice->AllocateImage( theImageResource, vk::ImageLayout::eShaderReadOnlyOptimal );
            mPresenter->SetImage( mImage );
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }


    // Don't scroll when interacting with the GUI
    ImGuiIO &io = ImGui::GetIO();
    if( io.WantCaptureMouse ) return;

    if( mInputState->IsButtonDown( GLFW_MOUSE_BUTTON_1 ) )
    {
        auto delta = mInputState->GetMouseDelta();
        mPresenter->GetPos() -= delta * mPresenter->GetScale();
    }

    float scroll = mInputState->GetMouseScrollDelta().y;
    if( scroll != 0.0f )
    {
        mPresenter->GetScale() -= scroll * 0.1f;
    }

}

