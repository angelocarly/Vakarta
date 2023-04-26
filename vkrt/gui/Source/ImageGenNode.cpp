//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/ImageGenNode.h"

#include "vks/render/VulkanSession.h"

#include <imnodes.h>

vkrt::gui::ImageGenNode::ImageGenNode( vkrt::gui::NodeContextPtr inContext )
:
    Node( inContext, Node::Type::ImageGen ),
    mReflectionPresenter( vks::VulkanSession::GetInstance() )
{
    mOutputId = inContext->AddAttribute();
}

vkrt::gui::ImageGenNode::~ImageGenNode()
{

}

void
vkrt::gui::ImageGenNode::Draw()
{
    ImNodes::BeginNode( GetId() );
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted( "Image gen" );
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginOutputAttribute( mOutputId );
        ImGui::Indent( 40 );
        ImGui::Text( "output" );
        ImNodes::EndOutputAttribute();

        if( ImGui::Button( "Edit.." ) )
        {
            mShowEditor = true;
        }

        mEditor.Draw( mShowEditor );

    }
    ImNodes::EndNode();

}

vk::DescriptorSet
vkrt::gui::ImageGenNode::ProvideImage()
{
    return mReflectionPresenter.GetImage();
}

