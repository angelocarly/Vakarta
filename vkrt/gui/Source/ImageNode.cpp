//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/ImageNode.h"
#include "vkrt/gui/ForwardDecl.h"

#include "vks/render/VksSession.h"
#include "vks/render/Device.h"
#include "vks/core/Image.h"
#include "imgui_impl_vulkan.h"

#include <optional>
#include <imnodes.h>

vkrt::gui::ImageNode::ImageNode( vkrt::gui::NodeContextPtr inContext )
:
    vkrt::gui::Node( -1 ),
    mInput( inContext->CreateInputAttribute< vks::Image >
    (
        [ this ]( const std::optional< vks::Image >& inImage ){ SetupImageViews( inImage ); },
        [ this ]( const std::optional< vks::Image >& inImage ){ DisconnectImageViews(); SetupImageViews( inImage ); },
        [](){}
    ) )
{
    LinkAttribute( mInput );
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

        if( !mInput->IsConnected() || !mInput->GetResource().has_value() )
        {
            ImGui::Text( "No image provider" );
        }
        else
        {
            ImVec2 wsize = ImVec2( 200, 200 );
            ImGui::Image( mDescriptorSet, wsize );
        }

    }
    ImNodes::EndNode();

}

void
vkrt::gui::ImageNode::SetupImageViews( std::optional< vks::Image > inImage )
{
    if( !inImage ) return;

    auto theDevice = vks::VksSession::GetInstance()->GetDevice();
    auto theImage = mInput->GetResource().value();

    // Initialize data
    auto imageViewCreateInfo = vk::ImageViewCreateInfo
    (
        vk::ImageViewCreateFlags(),
        theImage.GetVkImage(),
        vk::ImageViewType::e2D,
        vk::Format::eR8G8B8A8Unorm,
        vk::ComponentMapping
        (
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity
        ),
        vk::ImageSubresourceRange
        (
            vk::ImageAspectFlagBits::eColor,
            0,
            1,
            0,
            1
        )
    );
    mImageView = theDevice->GetVkDevice().createImageView( imageViewCreateInfo );

    mSampler = theDevice->GetVkDevice().createSampler
    (
        vk::SamplerCreateInfo
        (
            vk::SamplerCreateFlags(),
            vk::Filter::eNearest,
            vk::Filter::eNearest,
            vk::SamplerMipmapMode::eNearest,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            0.0f,
            VK_FALSE,
            16.0f,
            VK_FALSE,
            vk::CompareOp::eAlways,
            0.0f,
            0.0f,
            vk::BorderColor::eIntOpaqueBlack,
            VK_FALSE
        )
    );

    mDescriptorSet = ImGui_ImplVulkan_AddTexture( mSampler, mImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
}

void vkrt::gui::ImageNode::DisconnectImageViews()
{
    // Destroy resources
    auto theDevice = vks::VksSession::GetInstance()->GetDevice();
    theDevice->GetVkDevice().waitIdle();
    theDevice->GetVkDevice().destroy( mImageView );
    theDevice->GetVkDevice().destroy( mSampler );
    mImageView = nullptr;
    mSampler = nullptr;
    mDescriptorSet = nullptr;
}
