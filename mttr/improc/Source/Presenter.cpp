/**
 * Presenter.cpp
 *
 * @file	Presenter.cpp
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#include "mttr/improc/Presenter.h"

#include "vkrt/graphics/Presenter.h"

#include "vks/core/Buffer.h"
#include "vks/core/Image.h"
#include "vks/render/Device.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"
#include "vks/render/DescriptorLayoutBuilder.h"

#include <glm/vec3.hpp>

Mttr::Imp::Presenter::Presenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera )
:
    vkrt::Presenter( inDevice, inWidth, inHeight ),
    mDevice( inDevice ),
    mWidth( inWidth ),
    mHeight( inHeight ),
    mCamera( inCamera )
{
    InitializeDescriptorSetLayout();
    InitializeDisplayPipeline( GetRenderPass() );
    InitializeBuffers();
}

Mttr::Imp::Presenter::~Presenter()
{
    mDevice->GetVkDevice().destroy( mImageView );
    mDevice->GetVkDevice().destroy( mSampler );
    mDevice->GetVkDevice().destroy( mDescriptorSetLayout );
}

void
Mttr::Imp::Presenter::InitializeDescriptorSetLayout()
{
    mDescriptorSetLayout = vks::DescriptorLayoutBuilder()
        .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eCompute )
        .Build( mDevice, vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
        .front();
}


void
Mttr::Imp::Presenter::InitializeDisplayPipeline( vk::RenderPass const inRenderPass )
{
    auto theVertexShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/ScreenRect.vert.spv" );
    auto theFragmentShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/Improc.frag.spv" );

    std::vector< vk::PushConstantRange > thePushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eFragment,
            0,
            sizeof( PushConstants )
        )
    };

    vks::Pipeline::PipelineCreateInfo theCreateInfo =
    {
        inRenderPass,
        theVertexShader,
        theFragmentShader,
        { mDescriptorSetLayout },
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

void
Mttr::Imp::Presenter::InitializeBuffers()
{
}

void
Mttr::Imp::Presenter::Draw( const vkrt::RenderEnvironment & inRenderEnvironment )
{

    if( !mCanRender ) return;

    auto theCommandBuffer = inRenderEnvironment.mCommandBuffer;

    BeginPresenterRenderPass( theCommandBuffer );
    {
        theCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline->GetVkPipeline() );

        PushConstants thePushConstants
        {
            mSize,
            mPos,
            glm::vec4( mColorTreshold, 1.0f ),
            glm::vec4( mReplacementColor, 1.0f ),
            mScale
        };
        theCommandBuffer.pushConstants
        (
            mPipeline->GetVkPipelineLayout(),
            vk::ShaderStageFlagBits::eFragment,
            0,
            sizeof( PushConstants ),
            & thePushConstants
        );

        auto theImageInfo = vk::DescriptorImageInfo
        (
            mSampler,
            mImageView,
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
    EndPresenterRenderPass( theCommandBuffer );
}

void
Mttr::Imp::Presenter::SetupImageViews( vks::Image & inImage )
{
    if( mCanRender )
    {
        mDevice->GetVkDevice().destroy( mImageView );
        mDevice->GetVkDevice().destroy( mSampler );
    }

    mCanRender = true;

    mSize = glm::vec2( inImage.GetWidth(), inImage.GetHeight() );

    auto theDevice = vks::VksSession::GetInstance()->GetDevice();

    // Initialize data
    auto imageViewCreateInfo = vk::ImageViewCreateInfo
    (
        vk::ImageViewCreateFlags(),
        inImage.GetVkImage(),
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
}

void
Mttr::Imp::Presenter::SetImage( vks::Image & inImage )
{
    SetupImageViews( inImage );
}
