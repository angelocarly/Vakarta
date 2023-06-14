/**
 * LayerPresenter.cpp
 *
 * @file	LayerPresenter.cpp
 * @author	Angelo Carly
 * @date	13/06/2023
 *
 */

#include "vkrt/graphics/LayerPresenter.h"

#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorLayoutBuilder.h"

vkrt::LayerPresenter::LayerPresenter( vks::DevicePtr inDevice, vk::Extent2D inExtent )
:
    mDevice( inDevice ),
    mExtent( inExtent )
{
    InitializeRenderPass();
}

vkrt::LayerPresenter::~LayerPresenter()
{
    mDevice->GetVkDevice().destroyRenderPass( mRenderPass );
}

void
vkrt::LayerPresenter::InitializeRenderPass()
{
    vk::AttachmentDescription theColorAttachment =
    {
        {},
        vk::Format::eR8G8B8A8Unorm,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    };

    vk::AttachmentReference theColorAttachmentRef =
    {
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    };

    vk::SubpassDescription theSubpass =
    {
        {},
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        &theColorAttachmentRef
    };

    vk::SubpassDependency theDependency =
    {
        VK_SUBPASS_EXTERNAL,
        0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        {},
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite
    };

    std::array< vk::AttachmentDescription, 1 > theAttachments =
    {
        theColorAttachment
    };

    vk::RenderPassCreateInfo theCreateInfo =
    {
        {},
        static_cast< std::uint32_t >( theAttachments.size() ),
        theAttachments.data(),
        1,
        &theSubpass,
        1,
        &theDependency
    };

    mRenderPass = mDevice->GetVkDevice().createRenderPass( theCreateInfo );
}

void
vkrt::LayerPresenter::InitializePipeline( vk::RenderPass inRenderPass )
{
    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/ScreenRect.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/CompositeFrames.frag.spv" );

    auto theDescriptorLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR );

    vks::Pipeline::PipelineCreateInfo theCreateInfo =
    {
        inRenderPass,
        theVertexShader,
        theFragmentShader,
        { theDescriptorLayout },
        {}
    };

    vks::Pipeline::PipelineConfigInfo theConfigInfo =
    {
        vk::PrimitiveTopology::eTriangleList,
        {},
        {}
    };
    mPipeline = std::make_unique< vks::Pipeline >( mDevice, theCreateInfo, theConfigInfo );

    mDevice->GetVkDevice().destroy( theVertexShader );
    mDevice->GetVkDevice().destroy( theFragmentShader );
}

void
vkrt::LayerPresenter::Prepare( const vkrt::RenderEnvironment & inEnvironment )
{
    // Update the presenters
    for( auto theContext : mPresenterContexts )
    {
        theContext.mPresenter->Prepare( inEnvironment );
    }

    // Draw the presenters to their framebuffers
    vk::CommandBuffer theCommandBuffer = inEnvironment.mCommandBuffer;
    for( auto theContext : mPresenterContexts )
    {
        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            theContext.mImage,
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eShaderWrite,
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::DependencyFlagBits::eByRegion
        );

        // Set viewport and scissor
        auto theViewport = vk::Viewport
        (
            0,
            0,
            mExtent.width,
            mExtent.height,
            0.0f,
            1.0f
        );
        theCommandBuffer.setViewport( 0, 1, & theViewport );
        const auto theScissors = vk::Rect2D( { 0, 0 }, mExtent );
        theCommandBuffer.setScissor( 0, 1, & theScissors );

        auto theClearValue = vk::ClearValue( vk::ClearColorValue( std::array< float, 4 >{ 0.0f, 0.0f, 0.0f, 1.0f } ) );
        auto theBeginInfo = vk::RenderPassBeginInfo
        (
            mRenderPass,
            theContext.mFramebuffer,
            vk::Rect2D
            (
                vk::Offset2D( 0, 0 ),
                mExtent
            ),
            1,
            & theClearValue
        );
        theCommandBuffer.beginRenderPass( theBeginInfo, vk::SubpassContents::eInline );
        {
            // Construct a new environment referencing this renderpass
            RenderEnvironment theEnvironment = inEnvironment;
            theEnvironment.mResolution = mExtent;
            theEnvironment.mRenderPass = mRenderPass;

            // Draw to this framebuffer
            theContext.mPresenter->Draw( theEnvironment );
        }
        theCommandBuffer.endRenderPass();

        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            theContext.mImage,
            vk::AccessFlagBits::eColorAttachmentWrite,
            vk::AccessFlagBits::eNone,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::ImageLayout::ePresentSrcKHR,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::DependencyFlagBits::eByRegion
        );
    }
}

void
vkrt::LayerPresenter::Draw( const vkrt::RenderEnvironment & inEnvironment )
{
    // I'll assume the renderpass doesn't change
    if( !mPipeline ) InitializePipeline( inEnvironment.mRenderPass );

    for( auto theContext : mPresenterContexts )
    {
        auto theCommandBuffer = inEnvironment.mCommandBuffer;
        theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline->GetVkPipeline() );

        auto theImageInfo = vk::DescriptorImageInfo
        (
            theContext.mSampler,
            theContext.mImageView,
            vk::ImageLayout::eShaderReadOnlyOptimal
        );

        auto theWriteDescriptorSet = vk::WriteDescriptorSet();
        theWriteDescriptorSet.setDstBinding( 0 );
        theWriteDescriptorSet.setDstArrayElement( 0 );
        theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eCombinedImageSampler );
        theWriteDescriptorSet.setDescriptorCount( 1 );
        theWriteDescriptorSet.setPImageInfo( & theImageInfo );

        PFN_vkCmdPushDescriptorSetKHR pfnVkCmdPushDescriptorSetKhr = reinterpret_cast< PFN_vkCmdPushDescriptorSetKHR >( mDevice->GetVkDevice().getProcAddr( "vkCmdPushDescriptorSetKHR" ) );
        pfnVkCmdPushDescriptorSetKhr
        (
            theCommandBuffer,
            VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS,
            mPipeline->GetVkPipelineLayout(),
            0,
            1,
            reinterpret_cast< const VkWriteDescriptorSet * >(& theWriteDescriptorSet)
        );

        // Render
        theCommandBuffer.draw( 3, 1, 0, 0 );
    }
}

void
vkrt::LayerPresenter::AddPresenter( std::shared_ptr< vkrt::Presenter > inPresenter )
{
    PresenterContext theContext;

    theContext.mPresenter = inPresenter;

    theContext.mImage = mDevice->CreateImage
    (
        vk::Format::eR8G8B8A8Unorm,
        mExtent.width,
        mExtent.height,
        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
        vma::AllocationCreateFlagBits::eDedicatedMemory
    );

    auto theCommandBuffer = mDevice->BeginSingleTimeCommands();
    {
        mDevice->ImageMemoryBarrier
        (
            theCommandBuffer,
            theContext.mImage,
            vk::AccessFlagBits::eShaderRead,
            vk::AccessFlagBits::eColorAttachmentWrite,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::DependencyFlagBits::eByRegion
        );
    }
    mDevice->EndSingleTimeCommands( theCommandBuffer );

    theContext.mImageView = mDevice->GetVkDevice().createImageView
    (
        vk::ImageViewCreateInfo
        (
            {},
            theContext.mImage.GetVkImage(),
            vk::ImageViewType::e2D,
            vk::Format::eR8G8B8A8Unorm,
            {},
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

    theContext.mFramebuffer = mDevice->GetVkDevice().createFramebuffer
    (
        vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            1,
            & theContext.mImageView,
            mExtent.width,
            mExtent.height,
            1
        )
    );

    theContext.mSampler = mDevice->GetVkDevice().createSampler
    (
        vk::SamplerCreateInfo
        (
            {},
            vk::Filter::eNearest,
            vk::Filter::eNearest,
            vk::SamplerMipmapMode::eNearest,
            vk::SamplerAddressMode::eClampToEdge,
            vk::SamplerAddressMode::eClampToEdge,
            vk::SamplerAddressMode::eClampToEdge,
            0.0f,
            VK_FALSE,
            16.0f,
            VK_FALSE,
            vk::CompareOp::eNever,
            0.0f,
            0.0f,
            vk::BorderColor::eFloatOpaqueWhite,
            VK_FALSE
        )
    );

    mPresenterContexts.push_back( theContext );

}
