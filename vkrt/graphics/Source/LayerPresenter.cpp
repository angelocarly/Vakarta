/**
 * LayerPresenter.cpp
 *
 * @file	LayerPresenter.cpp
 * @author	Angelo Carly
 * @date	13/06/2023
 *
 */

#include "vkrt/graphics/LayerPresenter.h"

#include "vkrt/graphics/Presenter.h"

#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorLayoutBuilder.h"

vkrt::LayerPresenter::LayerPresenter( vks::DevicePtr inDevice, vk::Extent2D inExtent )
:
    vkrt::Presenter( inDevice, inExtent.width, inExtent.height ),
    mDevice( inDevice ),
    mExtent( inExtent )
{
    InitializePipeline( GetRenderPass() );
}

vkrt::LayerPresenter::~LayerPresenter()
{
    mDevice->GetVkDevice().destroy( mDescriptorLayout );
}

void
vkrt::LayerPresenter::InitializePipeline( vk::RenderPass inRenderPass )
{
    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/ScreenRect.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/CompositeFrames.frag.spv" );

    mDescriptorLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
        .front();

    vks::Pipeline::PipelineCreateInfo theCreateInfo =
    {
        inRenderPass,
        theVertexShader,
        theFragmentShader,
        { mDescriptorLayout },
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
vkrt::LayerPresenter::Draw( const vkrt::RenderEnvironment & inEnvironment )
{
    for( auto thePresenter: mPresenters )
    {
        thePresenter->Draw( inEnvironment );
    }

    BeginRenderPass( inEnvironment.mCommandBuffer );
    {
        for( auto thePresenter: mPresenters )
        {
            auto theCommandBuffer = inEnvironment.mCommandBuffer;
            theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline->GetVkPipeline() );

            auto theImageInfo = vk::DescriptorImageInfo
            (
                thePresenter->GetSampler(),
                thePresenter->GetImageView(),
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
    EndRenderPass( inEnvironment.mCommandBuffer );
}

void
vkrt::LayerPresenter::AddPresenter( vkrt::PresenterPtr inPresenter )
{
    mPresenters.push_back( inPresenter );
}
