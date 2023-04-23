/**
 * GuiNodes.cpp
 *
 * @file	GuiNodes.cpp
 * @author	Angelo Carly
 * @date	22/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "vkrt/gui/GuiNodes.h"

#include "vkrt/core/InputState.h"
#include "vkrt/gui/NodeContext.h"
#include "vkrt/gui/ImageNode.h"
#include "vkrt/gui/ImageGenNode.h"

#include <GLFW/glfw3.h>
#include <imnodes.h>

vkrt::GuiNodes::GuiNodes( vkrt::InputStatePtr inInputState )
:
    mInputState( inInputState )
{
    mNodeContext = std::make_shared< vkrt::gui::NodeContext >();
    mNodes.push_back( std::make_shared< vkrt::gui::ImageNode >( mNodeContext ) );
    mNodes.push_back( std::make_shared< vkrt::gui::ImageNode >( mNodeContext ) );
    mNodes.push_back( std::make_shared< vkrt::gui::ImageNode >( mNodeContext ) );
    mNodes.push_back( std::make_shared< vkrt::gui::ImageGenNode >( mNodeContext ) );
}

vkrt::GuiNodes::~GuiNodes()
{

}

void
vkrt::GuiNodes::Draw( vkrt::Presenter & inPresenter )
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 window_pos, window_pos_pivot;
    window_pos = work_pos;
    window_pos.x += 1500.0f;
    window_pos.y += 900.0f;

    ImGui::Begin("Nodes" );
    ImNodes::BeginNodeEditor();
    {

        ImNodes::PushColorStyle( ImNodesCol_TitleBar, IM_COL32( 233, 150, 29, 255 ) );
        ImNodes::PushColorStyle( ImNodesCol_TitleBarSelected, IM_COL32( 213, 149, 56, 255 ) );
        ImNodes::PushColorStyle( ImNodesCol_TitleBarHovered, IM_COL32( 213, 170, 108, 255 ) );

        for( auto theNode : mNodes )
        {
            theNode->Draw();
        }

        for( int i = 0; i < mNodeContext->GetLinks().size(); i++ )
        {
            auto link = mNodeContext->GetLinks()[ i ];
            ImNodes::Link( i, link.mA, link.mB );
        }

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
    }
    ImNodes::EndNodeEditor();
    ImGui::End();

    // Check if links have been created
    int start_attr, end_attr;
    if ( ImNodes::IsLinkCreated( &start_attr, &end_attr ) )
    {
        mNodeContext->AddLink( start_attr, end_attr );
    }

    std::vector< std::size_t > linksToRemove;
    for( auto theLink : mNodeContext->GetLinks() )
    {
        if( !ImNodes::IsLinkSelected( theLink.first ) ) continue;
        if( mInputState->IsKeyDown( GLFW_KEY_DELETE ) )
        {
            linksToRemove.push_back( theLink.first );
            break;
        }
    }

    for( auto theLink : linksToRemove )
    {
        mNodeContext->RemoveLink( theLink );
    }
}
