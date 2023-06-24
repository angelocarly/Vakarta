/**
 * TestPresenter.cpp
 *
 * @file	TestPresenter.cpp
 * @author	Angelo Carly
 * @date	11/06/2023
 *
 *
 */

#include "vkrt/graphics/TestPresenter.h"

#include "vks/render/DescriptorLayoutBuilder.h"
#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"

vkrt::TestPresenter::TestPresenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight )
:
    vkrt::Presenter( inDevice, inWidth, inHeight ),
    mDevice( inDevice )
{
    InitializePipeline( GetRenderPass() );

}

vkrt::TestPresenter::~TestPresenter()
{
}

void
vkrt::TestPresenter::InitializePipeline( vk::RenderPass const inRenderPass )
{
    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/ScreenRect.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/FrameIdentification.frag.spv" );

    std::vector< vk::PushConstantRange > thePushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eFragment,
            0,
            sizeof( std::uint32_t )
        )
    };

    vks::Pipeline::PipelineCreateInfo theCreateInfo =
    {
        inRenderPass,
        theVertexShader,
        theFragmentShader,
        {},
        thePushConstants
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

// ============================================= Rendering =============================================================

int theIndex = 0;
void
vkrt::TestPresenter::Draw( vkrt::RenderEnvironment const & inEnvironment )
{
    BeginPresenterRenderPass( inEnvironment.mCommandBuffer );
    {
        theIndex++;
        if( theIndex == 7 ) theIndex = 0;

        auto theCommandBuffer = inEnvironment.mCommandBuffer;

        // Bind
        theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline->GetVkPipeline() );

        // Update data
        theCommandBuffer.pushConstants
            (
                mPipeline->GetVkPipelineLayout(),
                vk::ShaderStageFlagBits::eFragment,
                0,
                sizeof( uint32_t ),
                & theIndex
            );

        // Render
        theCommandBuffer.draw( 3, 1, 0, 0 );
    }
    EndPresenterRenderPass( inEnvironment.mCommandBuffer );
}