//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/ImageNode.h"
#include "vkrt/gui/ForwardDecl.h"

#include <imnodes.h>

vkrt::gui::ImageNode::ImageNode( vkrt::gui::NodeContextPtr inContext )
:
    vkrt::gui::Node( inContext, vkrt::gui::Node::Type::ImageOutput )
{
    mInputId = inContext->AddAttribute();
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

        ImNodes::BeginInputAttribute( mInputId );
        ImNodes::EndInputAttribute();

        if( !mImageProvider )
        {
            ImGui::Text( "No image provider" );
        }
        else
        {
            ImVec2 wsize = ImVec2( 200, 200 );
            ImGui::Image( mImageProvider->ProvideImage(), wsize );
        }

    }
    ImNodes::EndNode();

}

void
vkrt::gui::ImageNode::SetImageProvider( std::shared_ptr< vkrt::gui::ImageProvider > inImageProvider )
{
    mImageProvider = inImageProvider;
}
