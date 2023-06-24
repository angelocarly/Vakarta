/**
 * GuiNodes.cpp
 *
 * @file	GuiNodes.cpp
 * @author	Angelo Carly
 * @date	22/04/2023
 *
 */

#include "vkrt/gui/GuiNodes.h"

#include "vkrt/core/InputState.h"

#include "vkrt/gui/BufferNode.h"
#include "vkrt/gui/ImageNode.h"
#include "vkrt/gui/Link.h"
#include "vkrt/gui/NodeContext.h"
#include "vkrt/gui/PNGNode.h"
#include "vkrt/gui/CrystalShaderNode.h"
#include "vkrt/gui/ShaderNode.h"
#include "vkrt/gui/FilterNode.h"

#include <GLFW/glfw3.h>
#include <imnodes.h>

vkrt::GuiNodes::GuiNodes( vkrt::InputStatePtr inInputState )
:
    mInputState( inInputState )
{
    mNodeContext = std::make_shared< vkrt::gui::NodeContext >();
}

vkrt::GuiNodes::~GuiNodes()
{

}

void
vkrt::GuiNodes::Draw( vkrt::Presenter & inPresenter )
{
    // Delete links before rendering
    std::vector< std::size_t > linksToRemove;
    for( auto theLink : mNodeContext->GetLinks() )
    {
        if( !ImNodes::IsLinkSelected( theLink.first ) ) continue;
        if( mInputState->IsKeyDown( GLFW_KEY_DELETE ) || mInputState->IsKeyDown( GLFW_KEY_BACKSPACE ) )
        {
            linksToRemove.push_back( theLink.first );
            break;
        }
    }
    for( auto theLinkId : linksToRemove )
    {
        mNodeContext->RemoveLink( theLinkId );
    }

    // Delete nodes before rendering
    std::vector< std::size_t > nodesToRemove;
    for( auto theNode : mNodeContext->GetNodes() )
    {
        if( !ImNodes::IsNodeSelected( theNode.first ) ) continue;
        if( mInputState->IsKeyDown( GLFW_KEY_DELETE ) || mInputState->IsKeyDown( GLFW_KEY_BACKSPACE ) )
        {
            nodesToRemove.push_back( theNode.first );
            break;
        }
    }
    for( auto theNodeId : nodesToRemove )
    {
        mNodeContext->RemoveNode( theNodeId );
    }

    // Render nodes
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 window_pos, window_pos_pivot;
    window_pos = work_pos;
    window_pos.x += 1500.0f;
    window_pos.y += 900.0f;

    ImGui::Begin("Nodes" );
    ImNodes::BeginNodeEditor();
    {

        if( ImGui::IsKeyReleased( ImGuiKey_A ) )
        {
            ImGui::OpenPopup( "add node" );
        }

        if( ImGui::BeginPopup( "add node" ) )
        {
            const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

            if( ImGui::MenuItem( "Image load" ) )
            {
                auto theId = mNodeContext->AddNode( std::make_shared< gui::PNGNode >( mNodeContext ) );
                ImNodes::SetNodeScreenSpacePos( theId, click_pos );
                ImGui::CloseCurrentPopup();
            }

            if( ImGui::MenuItem( "Buffer" ) )
            {
                auto theId = mNodeContext->AddNode( std::make_shared< gui::BufferNode >( mNodeContext ) );
                ImNodes::SetNodeScreenSpacePos( theId, click_pos );
                ImGui::CloseCurrentPopup();
            }

            if( ImGui::MenuItem( "Image view" ) )
            {
                auto theId = mNodeContext->AddNode( std::make_shared< gui::ImageNode >( mNodeContext ) );
                ImNodes::SetNodeScreenSpacePos( theId, click_pos );
                ImGui::CloseCurrentPopup();
            }

            if( ImGui::MenuItem( "Shader node" ) )
            {
                auto theId = mNodeContext->AddNode( std::make_shared< gui::ShaderNode >( mNodeContext ) );
                ImNodes::SetNodeScreenSpacePos( theId, click_pos );
                ImGui::CloseCurrentPopup();
            }

            if( ImGui::MenuItem( "Crystal view" ) )
            {
                auto theId = mNodeContext->AddNode( std::make_shared< gui::CrystalShaderNode >( mNodeContext ) );
                ImNodes::SetNodeScreenSpacePos( theId, click_pos );
                ImGui::CloseCurrentPopup();
            }

            if( ImGui::MenuItem( "Filter" ) )
            {
                auto theId = mNodeContext->AddNode( std::make_shared< gui::FilterNode >( mNodeContext ) );
                ImNodes::SetNodeScreenSpacePos( theId, click_pos );
                ImGui::CloseCurrentPopup();
            }


            ImGui::EndPopup();
        }

        ImNodes::PushColorStyle( ImNodesCol_TitleBar, IM_COL32( 233, 150, 29, 255 ) );
        ImNodes::PushColorStyle( ImNodesCol_TitleBarSelected, IM_COL32( 213, 149, 56, 255 ) );
        ImNodes::PushColorStyle( ImNodesCol_TitleBarHovered, IM_COL32( 213, 170, 108, 255 ) );

        // Draw the nodes
        mNodeContext->Traverse
        (
            [ this ]( std::shared_ptr< gui::Node > & inNode )
            {
                inNode->Draw();
            }
        );

        for( auto theLink: mNodeContext->GetLinks() )
        {
            ImNodes::Link( theLink.first, theLink.second.mSrcAttribute, theLink.second.mDstAttribute );
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
        if( mNodeContext->GetAttributes()[ start_attr ]->mResourceType == mNodeContext->GetAttributes()[ end_attr ]->mResourceType )
        {
            mNodeContext->AddLink( start_attr, end_attr );
        }
    }

}
