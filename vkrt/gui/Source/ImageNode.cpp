//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/ImageNode.h"
#include "vkrt/gui/ForwardDecl.h"

#include <imnodes.h>

vkrt::gui::ImageNode::ImageNode( vkrt::gui::NodeContextPtr inContext )
:
    vkrt::gui::Node( inContext ),
    mInput( inContext->AddInputAttribute() )
{
}

vkrt::gui::ImageNode::~ImageNode()
{

}

void
vkrt::gui::ImageNode::Draw()
{
    ImNodes::BeginNode( GetId() );
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted( "Image output" );
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute( mInput->mId );
        ImNodes::EndInputAttribute();

        if( !mInput->GetResource() )
        {
            ImGui::Text( "No image provider" );
        }
        else
        {
            ImVec2 wsize = ImVec2( 200, 200 );
            ImGui::Image( mInput->GetResource(), wsize );
        }

    }
    ImNodes::EndNode();

}
