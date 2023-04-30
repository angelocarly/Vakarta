/**
 * PNGNode.cpp
 *
 * @file	PNGNode.cpp
 * @author	Angelo Carly
 * @date	29/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "vkrt/gui/PNGNode.h"

#include "vks/assets/AssetLoader.h"
#include "vks/render/VulkanSession.h"
#include "vks/render/Device.h"

#include <imnodes.h>
#include <ImGuiFileDialog.h>

vkrt::gui::PNGNode::PNGNode( vkrt::gui::NodeContextPtr inContext )
:
    Node( inContext ),
    mOutput( inContext->AddOutputAttribute< vks::Image >( [ this ](){ return GetImage(); } ) )
{
}

vkrt::gui::PNGNode::~PNGNode()
{

}

void
vkrt::gui::PNGNode::Draw()
{
    ImNodes::BeginNode( GetId() );
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted( "PNG loader" );
        ImNodes::EndNodeTitleBar();

        if( mImage.GetVkImage() )
        {
            ImNodes::BeginOutputAttribute( mOutput->mId );
            ImGui::Indent( 40 );
            ImGui::Text( "%s", mFileName.c_str() );
            ImNodes::EndOutputAttribute();
        }
        else
        {
            ImGui::Text( "No image" );
        }

        if( ImGui::Button( "Load PNG.." ) )
        {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", ".");
        }

        // display
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                mFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
                mFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
                // action
                auto theImageResource = vks::AssetLoader::LoadImageResource( mFilePath );
                auto theDevice = vks::VulkanSession::GetInstance()->GetDevice();
                mImage = theDevice->AllocateImage( theImageResource, vk::ImageLayout::eShaderReadOnlyOptimal );
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }
    ImNodes::EndNode();
}

vks::Image
vkrt::gui::PNGNode::GetImage()
{
    return mImage;
}