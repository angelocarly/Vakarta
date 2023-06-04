//
// Created by magnias on 6/4/23.
//

#include "vkrt/graphics/GuiLayer.h"

#include "vks/render/Device.h"

vkrt::GuiLayer::GuiLayer( vks::DevicePtr inDevice )
:
    mDevice( inDevice )
{
//    auto theColorAttachmentReference = vk::AttachmentReference
//    (
//        0,
//        vk::ImageLayout::eColorAttachmentOptimal
//    );
//    auto theDepthStencilAttachmentReference = vk::AttachmentReference
//    (
//        1,
//        vk::ImageLayout::eDepthStencilAttachmentOptimal
//    );
//    auto theSubpassDescription = vk::SubpassDescription
//    (
//        vk::SubpassDescriptionFlags(),
//        vk::PipelineBindPoint::eGraphics,
//        0,
//        nullptr,
//        1,
//        &theColorAttachmentReference,
//        nullptr,
//        &theDepthStencilAttachmentReference,
//        0,
//        nullptr
//    );
//
//    // The depth attachment is first accessed in the early fragment stage.
//    std::array< vk::SubpassDependency, 1 > theSubPassDependencies;
//    theSubPassDependencies[ 0 ].setSrcSubpass( VK_SUBPASS_EXTERNAL );
//    theSubPassDependencies[ 0 ].setDstSubpass( 0 );
//    theSubPassDependencies[ 0 ].setSrcStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
//    theSubPassDependencies[ 0 ].setSrcAccessMask( vk::AccessFlagBits::eNone );
//    theSubPassDependencies[ 0 ].setDstStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
//    theSubPassDependencies[ 0 ].setDstAccessMask( vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite );
//
//    auto theColorAttachmentDescription = vk::AttachmentDescription
//    (
//        vk::AttachmentDescriptionFlags(),
//        vk::Format::eR8G8B8A8Unorm,
//        vk::SampleCountFlagBits::e1,
//        vk::AttachmentLoadOp::eClear,
//        vk::AttachmentStoreOp::eStore,
//        vk::AttachmentLoadOp::eDontCare,
//        vk::AttachmentStoreOp::eDontCare,
//        vk::ImageLayout::eUndefined,
//        vk::ImageLayout::ePresentSrcKHR
//    );
//
//    auto theDepthAttachmentDescription = vk::AttachmentDescription
//    (
//        vk::AttachmentDescriptionFlags(),
//        vk::Format::eR16Unorm,
//        vk::SampleCountFlagBits::e1,
//        vk::AttachmentLoadOp::eClear,
//        vk::AttachmentStoreOp::eStore,
//        vk::AttachmentLoadOp::eDontCare,
//        vk::AttachmentStoreOp::eDontCare,
//        vk::ImageLayout::eUndefined,
//        vk::ImageLayout::eDepthStencilAttachmentOptimal
//    );
//
//    std::vector< vk::AttachmentDescription > theAttachmentDescriptions =
//    {
//        theColorAttachmentDescription,
//        theDepthAttachmentDescription
//    };
//    auto theRenderPassCreateInfo = vk::RenderPassCreateInfo
//    (
//        vk::RenderPassCreateFlags(),
//        theAttachmentDescriptions.size(),
//        theAttachmentDescriptions.data(),
//        1,
//        &theSubpassDescription,
//        theSubPassDependencies.size(),
//        theSubPassDependencies.data()
//    );
//
//    mRenderPass = mDevice->GetVkDevice().createRenderPass( theRenderPassCreateInfo );
}

vkrt::GuiLayer::~GuiLayer()
{

}

void
vkrt::GuiLayer::Render( vkrt::RenderEnvironment & inRenderEnvironment )
{
    auto theCommandBuffer = inRenderEnvironment.mCommandBuffer;

//    vk::RenderPassBeginInfo theBeginInfo
//    (
//        mRenderPass,
//        inRenderEnvironment.mFrameBuffer,
//        inRenderEnvironment.mResolution,
//        {}
//    );
//    theCommandBuffer.beginRenderPass( theBeginInfo, vk::SubpassContents::eInline );
//    {
//        // Set viewport and scissor
//        auto theViewport = vk::Viewport
//            (
//                0,
//                0,
//                mSwapChain->GetExtent().width,
//                mSwapChain->GetExtent().height,
//                0.0f,
//                1.0f
//            );
//        theCommandBuffer.setViewport( 0, 1, & theViewport );
//        const auto theScissors = vk::Rect2D( { 0, 0 }, mSwapChain->GetExtent() );
//        theCommandBuffer.setScissor( 0, 1, & theScissors );
//
//        theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mDisplayPipeline->GetVkPipeline() );
//
//        // Draw each layer
//        for( auto & theLayer : theFrameContext.mLayers )
//        {
//            // Bind the layer's image
//            auto theImageInfo = vk::DescriptorImageInfo
//                (
//                    theLayer.mSampler,
//                    theLayer.mImageView,
//                    vk::ImageLayout::eShaderReadOnlyOptimal
//                );
//
//            auto theWriteDescriptorSet = vk::WriteDescriptorSet();
//            theWriteDescriptorSet.setDstBinding( 0 );
//            theWriteDescriptorSet.setDstArrayElement( 0 );
//            theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eCombinedImageSampler );
//            theWriteDescriptorSet.setDescriptorCount( 1 );
//            theWriteDescriptorSet.setPImageInfo( &theImageInfo );
//
//            PFN_vkCmdPushDescriptorSetKHR pfnVkCmdPushDescriptorSetKhr = reinterpret_cast< PFN_vkCmdPushDescriptorSetKHR >( mDevice->GetVkDevice().getProcAddr( "vkCmdPushDescriptorSetKHR" ) );
//            pfnVkCmdPushDescriptorSetKhr
//                (
//                    theCommandBuffer,
//                    VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
//                    mDisplayPipeline->GetVkPipelineLayout(),
//                    0,
//                    1,
//                    reinterpret_cast< const VkWriteDescriptorSet * >(& theWriteDescriptorSet)
//                );
//
//            // Render to screen
//            theCommandBuffer.draw( 3, 1, 0, 0 );
//        }
//    }
//    theCommandBuffer.endRenderPass();
}
