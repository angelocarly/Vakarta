/**
 * CrystalShaderNode.cpp
 * A shader node.
 * Well actually it renders a shader I am still creating.
 *
 * @file	CrystalShaderNode.cpp
 * @author	Angelo Carly
 * @date	08/05/2023
 *
 */

#include "vkrt/gui/CrystalShaderNode.h"

#include "vkrt/gui/ForwardDecl.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/VksSession.h"
#include "vks/render/RenderPass.h"
#include "vks/render/OffScreenRenderPass.h"

#include <imnodes.h>

vkrt::gui::CrystalShaderNode::CrystalShaderNode( vkrt::gui::NodeContextPtr inContext )
:
    vkrt::gui::Node( -1 ),
    mInput( inContext->CreateInputAttribute< float >
    (
        [ this ]( const std::optional< float > & inFloat ){},
        [ this ]( const std::optional< float > & inFloat ){},
        [](){}
    ) )
{
    LinkAttribute( mInput );

    auto theSession = vks::VksSession::GetInstance();

    mRenderPass = std::make_shared< vks::OffScreenRenderPass >
    (
        theSession->GetDevice(),
        1,
        vk::Format::eR8G8B8A8Unorm,
        800,
        800
    );
//    mShader = std::make_shared< vkrt::graphics::CrystalViewShader >( theSession, inContext->GetRenderPass() );
}

vkrt::gui::CrystalShaderNode::~CrystalShaderNode()
{

}

void
vkrt::gui::CrystalShaderNode::Draw()
{
    ImNodes::BeginNode( GetId() );
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted( "Crystal view" );
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute( mInput->mId );
        ImNodes::EndInputAttribute();

        if( !mInput->IsConnected() || !mInput->GetResource().has_value() )
        {
            ImGui::Text( "No image provider" );
        }
        else
        {
            ImVec2 wsize = ImVec2( 200, 200 );
//            ImGui::Image( mDescriptorSet, wsize );
        }

    }
    ImNodes::EndNode();
}
