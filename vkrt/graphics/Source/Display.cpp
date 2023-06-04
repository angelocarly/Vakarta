//
// Created by Angelo Carly on 23/11/2022.
//
#include "vkrt/graphics/Display.h"

#include "vkrt/graphics/RenderEnvironment.h"
#include "vkrt/graphics/Layer.h"

#include "vks/render/ForwardDecl.h"
#include "vks/render/RenderPass.h"
#include "vks/render/Swapchain.h"
#include "vks/render/Window.h"
#include "vks/render/Utils.h"
#include "vkrt/graphics/GuiLayer.h"
#include "vks/render/DescriptorLayoutBuilder.h"

#include <spdlog/spdlog.h>

#include <memory>

vkrt::Display::Display( vks::VulkanSessionPtr inSession, vks::WindowPtr inWindow )
:
    mWindow( inWindow ),
    mDevice( inSession->GetDevice() ),
    mSwapChain( std::make_shared< vks::Swapchain >( mDevice, mWindow->GetVkSurface() ) ),
    mRenderPass( std::make_shared< vks::RenderPass >( mSwapChain ) )
{
    auto theCommandBufferCount = mSwapChain->GetImageCount();
    for( int i = 0; i < theCommandBufferCount; i++ )
    {
        FrameContext theFrameContext;
        theFrameContext.mCommandBuffer = mDevice->GetVkDevice().allocateCommandBuffers
        (
            vk::CommandBufferAllocateInfo( mDevice->GetVkCommandPool(), vk::CommandBufferLevel::ePrimary, 1 )
        ).front();
        mFrameContext.push_back( theFrameContext );
    }

    InitializePipeline();

    LayerPtr theLayer = std::make_shared< vkrt::GuiLayer >( mDevice );
    RegisterLayer( theLayer );
}

vkrt::Display::~Display()
{
    for( auto & theFrameContext : mFrameContext )
    {
        for( auto & theLayer : theFrameContext.mLayers )
        {
            mDevice->GetVkDevice().destroy( theLayer.mImageView );
            mDevice->GetVkDevice().destroy( theLayer.mSampler );
            mDevice->DestroyImage( theLayer.mImage );
        }
    }
}

void
vkrt::Display::InitializePipeline()
{
    mImageDescriptorLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
        .front();

    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/Combine.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/Combine.frag.spv" );

    vks::Pipeline::PipelineCreateInfo theCreateInfo =
    {
        mRenderPass->GetVkRenderPass(),
        theVertexShader,
        theFragmentShader,
        { mImageDescriptorLayout },
        {}
    };

    vks::Pipeline::PipelineConfigInfo theConfigInfo =
    {
        vk::PrimitiveTopology::eTriangleList,
        {},
        {}
    };
    mDisplayPipeline = std::make_unique< vks::Pipeline >( mDevice, theCreateInfo, theConfigInfo );

    mDevice->GetVkDevice().destroy( theVertexShader );
    mDevice->GetVkDevice().destroy( theFragmentShader );

}

// =====================================================================================================================

/**
 * @brief Draw a single frame to the screen
 * @param inMesh
 */
void
vkrt::Display::Render()
{
    uint32_t theImageIndex = mSwapChain->RetrieveNextImage();
    auto theFrameContext = mFrameContext[ theImageIndex ];

    // Begin the current frame's draw commands
    auto theCommandBuffer = theFrameContext.mCommandBuffer;
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {
        // Update the layers
        for( auto & theLayer : theFrameContext.mLayers )
        {
            vkrt::RenderEnvironment theEnvironment =
            {
                theLayer.mImage,
                vk::Rect2D( mSwapChain->GetExtent().width, mSwapChain->GetExtent().height ),
                theLayer.mImageView,
                theCommandBuffer
            };

            // Transition the image to color attachment optimal
            mDevice->ImageMemoryBarrier
            (
                theCommandBuffer,
                theLayer.mImage,
                vk::AccessFlagBits::eNone,
                vk::AccessFlagBits::eTransferWrite,
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eTransfer,
                theLayer.mImageLayout,
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::DependencyFlagBits::eByRegion
            );
            theLayer.mImageLayout = vk::ImageLayout::eColorAttachmentOptimal;

            theLayer.mLayer->Render( theEnvironment );

            // Transition the image to
            mDevice->ImageMemoryBarrier
            (
                theCommandBuffer,
                theLayer.mImage,
                vk::AccessFlagBits::eColorAttachmentWrite,
                vk::AccessFlagBits::eShaderRead,
                vk::PipelineStageFlagBits::eColorAttachmentOutput,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::ImageLayout::eShaderReadOnlyOptimal,
                vk::DependencyFlagBits::eByRegion
            );
            theLayer.mImageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        }

        // Renderpass to combine the layer's images into the final composition
        theCommandBuffer.beginRenderPass( mRenderPass->GetVkBeginInfo( theImageIndex ), vk::SubpassContents::eInline );
        {
            // Set viewport and scissor
            auto theViewport = vk::Viewport
            (
                0,
                0,
                mSwapChain->GetExtent().width,
                mSwapChain->GetExtent().height,
                0.0f,
                1.0f
            );
            theCommandBuffer.setViewport( 0, 1, & theViewport );
            const auto theScissors = vk::Rect2D( { 0, 0 }, mSwapChain->GetExtent() );
            theCommandBuffer.setScissor( 0, 1, & theScissors );

            theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mDisplayPipeline->GetVkPipeline() );

            // Draw each layer
            for( auto & theLayer : theFrameContext.mLayers )
            {
                // Bind the layer's image
                auto theImageInfo = vk::DescriptorImageInfo
                (
                    theLayer.mSampler,
                    theLayer.mImageView,
                    vk::ImageLayout::eShaderReadOnlyOptimal
                );

                auto theWriteDescriptorSet = vk::WriteDescriptorSet();
                theWriteDescriptorSet.setDstBinding( 0 );
                theWriteDescriptorSet.setDstArrayElement( 0 );
                theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eCombinedImageSampler );
                theWriteDescriptorSet.setDescriptorCount( 1 );
                theWriteDescriptorSet.setPImageInfo( &theImageInfo );

                PFN_vkCmdPushDescriptorSetKHR pfnVkCmdPushDescriptorSetKhr = reinterpret_cast< PFN_vkCmdPushDescriptorSetKHR >( mDevice->GetVkDevice().getProcAddr( "vkCmdPushDescriptorSetKHR" ) );
                pfnVkCmdPushDescriptorSetKhr
                (
                    theCommandBuffer,
                    VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
                    mDisplayPipeline->GetVkPipelineLayout(),
                    0,
                    1,
                    reinterpret_cast< const VkWriteDescriptorSet * >(& theWriteDescriptorSet)
                );

                // Render to screen
                theCommandBuffer.draw( 3, 1, 0, 0 );
            }
        }
        theCommandBuffer.endRenderPass();
    }
    theCommandBuffer.end();

    mSwapChain->SubmitCommandBuffer( theImageIndex, theCommandBuffer );
}

void
vkrt::Display::RegisterLayer( vkrt::LayerPtr inLayer )
{
    for( auto & theFrameContext : mFrameContext )
    {

        // Create the layer's image
        auto theLayerImage = mDevice->CreateImage
        (
            vk::Format::eR8G8B8A8Unorm,
            mSwapChain->GetExtent().width,
            mSwapChain->GetExtent().height,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            vma::AllocationCreateFlagBits::eDedicatedMemory
        );

        auto theLayerImageView = mDevice->GetVkDevice().createImageView
        (
            vk::ImageViewCreateInfo
            (
                vk::ImageViewCreateFlags(),
                theLayerImage.GetVkImage(),
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

        auto theLayerSampler = mDevice->GetVkDevice().createSampler
        (
            vk::SamplerCreateInfo
            (
                vk::SamplerCreateFlags(),
                vk::Filter::eLinear,
                vk::Filter::eLinear,
                vk::SamplerMipmapMode::eLinear,
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

        // Create a new render environment for the layer
        LayerContext theLayerContext =
        {
            vk::ImageLayout::eUndefined,
            theLayerImage,
            theLayerImageView,
            theLayerSampler,
            inLayer
        };

        theFrameContext.mLayers.push_back( theLayerContext );
    }
}

