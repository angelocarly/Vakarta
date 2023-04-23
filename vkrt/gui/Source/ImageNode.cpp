//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/ImageNode.h"
#include "vkrt/gui/ForwardDecl.h"

#include <imnodes.h>

vkrt::gui::ImageNode::ImageNode( vkrt::gui::NodeContextPtr inContext )
:
    vkrt::gui::Node( inContext )
{
    mInputId = inContext->AddAttribute();
    mOutputId = inContext->AddAttribute();
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
        ImGui::TextUnformatted( "simple node :)" );
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute( mInputId );
        ImGui::Text( "input" );
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute( mOutputId );
        ImGui::Indent( 40 );
        ImGui::Text( "output I" );
        ImNodes::EndOutputAttribute();
    }
    ImNodes::EndNode();

}
