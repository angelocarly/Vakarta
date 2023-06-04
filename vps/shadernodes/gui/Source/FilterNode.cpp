/**
 * Filter.cpp
 *
 * Simple post processing node
 *
 * @file	Filter.cpp
 * @author	Angelo Carly
 * @date	12/05/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "vkrt/gui/FilterNode.h"

#include "vkrt/gui/NodeContext.h"
#include "vkrt/graphics/FilterShader.h"

#include "vks/core/Image.h"
#include "vks/render/Device.h"
#include "vks/render/RenderPass.h"
#include "vks/render/VksSession.h"

#include <imgui.h>
#include <imnodes.h>

vkrt::gui::FilterNode::FilterNode( NodeContextPtr inContext  )
:
    mOutput( inContext->CreateOutputAttribute< vks::Image >
    (
        [ this ](){ return mImage; }
    ) ),
    mInput( inContext->CreateInputAttribute< vks::Image >
    (
        [ this ]( const std::optional< vks::Image >& inImage ){ SetupImageViews( inImage ); },
        [ this ]( const std::optional< vks::Image >& inImage ){ DisconnectImageViews(); SetupImageViews( inImage ); },
        [](){}
    ) )
{
    this->LinkAttribute( mInput );
    this->LinkAttribute( mOutput );

    mImageSize = inContext->GetDefaultImageSize();

    InitializeRenderTargets();
    mPipeline = std::make_shared< vkrt::graphics::FilterShader >( vks::VksSession::GetInstance(), mRenderPass );
}

vkrt::gui::FilterNode::~FilterNode()
{
    auto mSession = vks::VksSession::GetInstance();
    auto mDevice = mSession->GetDevice();

    mDevice->GetVkDevice().destroyImageView( mImageView );
    mDevice->GetVkDevice().destroyFramebuffer( mFramebuffer );
    mDevice->GetVkDevice().destroyRenderPass( mRenderPass );
    mDevice->DestroyImage( mImage );
}

void
vkrt::gui::FilterNode::InitializeRenderTargets()
{
    auto mSession = vks::VksSession::GetInstance();
    auto mDevice = mSession->GetDevice();

    // Image
    mImage = mDevice->CreateImage
    (
        vk::Format::eR8G8B8A8Unorm,
        mImageSize.x,
        mImageSize.y,
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eSampled,
        vma::AllocationCreateFlagBits::eDedicatedMemory
    );

    // Imageview
    mImageView = mDevice->GetVkDevice().createImageView
    (
        vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            mImage.GetVkImage(),
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
        )
    );

    // Renderpass
    auto theAttachmentDescription = vk::AttachmentDescription
    (
    vk::AttachmentDescriptionFlags(),
        vk::Format::eR8G8B8A8Unorm,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal
    );
    std::vector< vk::AttachmentDescription > theAttachmentDescriptions = { theAttachmentDescription };

    std::vector< vk::AttachmentReference > theAttachmentReferences =
    {
        vk::AttachmentReference
        (
            0,
            vk::ImageLayout::eColorAttachmentOptimal
        )
    };

    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        {},
        theAttachmentReferences,
        {},
        {}
    );
    std::vector< vk::SubpassDescription > theSubpassDescriptions = { theSubpassDescription };

    auto theSubpassDependency = vk::SubpassDependency
    (
        VK_SUBPASS_EXTERNAL,
        0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::DependencyFlagBits::eByRegion
    );
    std::vector< vk::SubpassDependency > theSubpassDependencies = { theSubpassDependency };

    mRenderPass = mDevice->GetVkDevice().createRenderPass
    (
        vk::RenderPassCreateInfo
        (
            vk::RenderPassCreateFlags(),
            theAttachmentDescriptions,
            theSubpassDescriptions,
            theSubpassDependencies
        )
    );

    mFramebuffer = mDevice->GetVkDevice().createFramebuffer
    (
        vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            1,
            &mImageView,
            mImageSize.x,
            mImageSize.y,
            1
        )
    );

    auto theCommandBuffer = mDevice->BeginSingleTimeCommands();
    {
        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            mImage,
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eTransferWrite,
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::DependencyFlagBits::eByRegion
        );
    }
    mDevice->EndSingleTimeCommands( theCommandBuffer );
}

void
vkrt::gui::FilterNode::Draw()
{
    ImNodes::BeginNode( GetId() );
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted( "Filter" );
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute( mInput->mId );
            ImGui::Text( "Input" );
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute( mOutput->mId );
            ImGui::Indent( 10 );
            ImGui::Text( "Output" );
        ImNodes::EndOutputAttribute();

        ImGui::PushItemWidth( 100.0f );

        ImNodes::EndNode();
    }

    // Render
    if( mInput->IsConnected() )
    {
        auto mSession = vks::VksSession::GetInstance();
        auto mDevice = mSession->GetDevice();

        auto theCommandBuffer = mDevice->BeginSingleTimeCommands();
        {
            // Change to general
            mDevice->ImageMemoryBarrier
            (
                theCommandBuffer,
                mImage,
                vk::AccessFlagBits::eNone,
                vk::AccessFlagBits::eTransferWrite,
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eTransfer,
                vk::ImageLayout::eShaderReadOnlyOptimal,
                vk::ImageLayout::eGeneral,
                vk::DependencyFlagBits::eByRegion
            );

            mDevice->ImageMemoryBarrier
            (
                theCommandBuffer,
                mInput->GetResource().value(),
                vk::AccessFlagBits::eNone,
                vk::AccessFlagBits::eTransferWrite,
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eTransfer,
                vk::ImageLayout::eShaderReadOnlyOptimal,
                vk::ImageLayout::eGeneral,
                vk::DependencyFlagBits::eByRegion
            );

            mPipeline->Bind( theCommandBuffer );
            mPipeline->BindImage( mInputImageView, mImageView );
            mPipeline->Dispatch( theCommandBuffer, mImageSize.x, mImageSize.y );

            // Change to read only
            mDevice->ImageMemoryBarrier
            (
                theCommandBuffer,
                mImage,
                vk::AccessFlagBits::eShaderWrite,
                vk::AccessFlagBits::eShaderRead,
                vk::PipelineStageFlagBits::eComputeShader,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::ImageLayout::eGeneral,
                vk::ImageLayout::eShaderReadOnlyOptimal,
                vk::DependencyFlagBits::eByRegion
            );

            mDevice->ImageMemoryBarrier
            (
                theCommandBuffer,
                mInput->GetResource().value(),
                vk::AccessFlagBits::eShaderWrite,
                vk::AccessFlagBits::eShaderRead,
                vk::PipelineStageFlagBits::eComputeShader,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::ImageLayout::eGeneral,
                vk::ImageLayout::eShaderReadOnlyOptimal,
                vk::DependencyFlagBits::eByRegion
            );
        }
        mDevice->EndSingleTimeCommands( theCommandBuffer );
    }
}

void
vkrt::gui::FilterNode::SetupImageViews( std::optional< vks::Image > inImage )
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
    mInputImageView = theDevice->GetVkDevice().createImageView( imageViewCreateInfo );
}

void
vkrt::gui::FilterNode::DisconnectImageViews()
{
    // Destroy resources
    auto theDevice = vks::VksSession::GetInstance()->GetDevice();
    theDevice->GetVkDevice().waitIdle();
    theDevice->GetVkDevice().destroy( mInputImageView );
    mInputImageView = nullptr;
}
