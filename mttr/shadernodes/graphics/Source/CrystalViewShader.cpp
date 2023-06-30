/**
 * CrystalViewShader.cpp
 *
 * @file	CrystalViewShader.cpp
 * @author	Angelo Carly
 * @date	08/05/2023
 *
 */

#include <cmath>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "vkrt/graphics/CrystalViewShader.h"

#include "vks/render/ComputePipeline.h"
#include "vks/render/Device.h"
#include "vks/render/DescriptorBinder.h"
#include "vks/render/Pipeline.h"
#include "vks/render/Utils.h"
#include "vks/render/VksSession.h"
#include "vks/render/RenderPass.h"
#include "vks/render/DescriptorLayoutBuilder.h"
#include "vks/render/DescriptorBinder.h"

class vkrt::graphics::CrystalViewShader::Impl
{
    public:
        Impl( vks::VksSessionPtr inSession, vk::RenderPass inRenderPass )
        :
            mSession( inSession ),
            mDevice( inSession->GetDevice() ),
            mRenderPass( inRenderPass )
        {
            InitializeDescriptors();
            InitializePipeline();
        }

        ~Impl()
        {
            for( auto theDescriptorSetLayout : mDescriptorSetLayouts )
            {
                mDevice->GetVkDevice().destroy( theDescriptorSetLayout );
            }
            mDevice->GetVkDevice().destroy( mDescriptorPool );
        }

    public:
        vks::VksSessionPtr mSession;
        vks::DevicePtr mDevice;
        vk::RenderPass mRenderPass;
        vks::ComputePipelinePtr mPipeline;
        vk::DescriptorPool mDescriptorPool;
        std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;
        vks::DescriptorSetPtr mDescriptorSet;

    public:
        struct PushConstantUniformObject
        {
            int mWidth;
            int mHeight;
            float mTScale;
            int mPadding2;
            glm::vec4 mColorA;
            glm::vec4 mColorB;
            glm::vec4 mColorC;
            glm::vec4 mColorD;
        };

    public:

    public:

        void InitializeDescriptors()
        {
            // Pool
            std::array< vk::DescriptorPoolSize, 1 > thePoolSizes = {};
            thePoolSizes[ 0 ].setType( vk::DescriptorType::eStorageImage );
            thePoolSizes[ 0 ].setDescriptorCount( 2 );

            vk::DescriptorPoolCreateInfo const theDescriptorPoolCreateInfo
            (
                vk::DescriptorPoolCreateFlags(),
                2,
                thePoolSizes.size(),
                thePoolSizes.data()
            );
            mDescriptorPool = mDevice->GetVkDevice().createDescriptorPool( theDescriptorPoolCreateInfo );

            // Layout
            mDescriptorSetLayouts = vks::DescriptorLayoutBuilder()
                .AddLayoutBinding( 0, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eCompute )
                .Build( mDevice );

            // Descriptor set
            mDescriptorSet = std::make_shared< vks::DescriptorSet >( mDevice, mDescriptorPool, mDescriptorSetLayouts.front() );
        }

        void InitializePipeline()
        {
            auto theComputeShader = vks::Utils::CreateVkShaderModule( mDevice, "shaders/CrystalView.comp.spv" );

            std::vector< vk::PushConstantRange > thePushConstants =
            {
                vk::PushConstantRange
                (
                    vk::ShaderStageFlagBits::eCompute,
                    0,
                    sizeof( PushConstantUniformObject )
                )
            };

            vks::ComputePipeline::ComputePipelineCreateInfo theCreateInfo =
            {
                mRenderPass,
                theComputeShader,
                mDescriptorSetLayouts,
                thePushConstants
            };
            mPipeline = std::make_unique< vks::ComputePipeline >( mDevice, theCreateInfo );

            mDevice->GetVkDevice().destroy( theComputeShader );
        }
};

vkrt::graphics::CrystalViewShader::CrystalViewShader( vks::VksSessionPtr inSession, vk::RenderPass inRenderPass )
:
    mImpl( new Impl( inSession, inRenderPass ) )
{

}

vkrt::graphics::CrystalViewShader::~CrystalViewShader()
{

}

void
vkrt::graphics::CrystalViewShader::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eCompute, mImpl->mPipeline->GetVkPipeline(), {} );
    inCommandBuffer.bindDescriptorSets( vk::PipelineBindPoint::eCompute, mImpl->mPipeline->GetVkPipelineLayout(), 0, { mImpl->mDescriptorSet->GetVkDescriptorSet() }, {} );
}

void
vkrt::graphics::CrystalViewShader::Dispatch( vk::CommandBuffer inCommandBuffer, std::size_t inWidth, std::size_t inHeight, glm::vec3 inA, glm::vec3 inB, glm::vec3 inC, glm::vec3 inD, float inTScale )
{
    std::chrono::milliseconds theTime =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (
            std::chrono::system_clock::now().time_since_epoch()
        );

    Impl::PushConstantUniformObject thePushConstants;
    thePushConstants.mWidth = inWidth;
    thePushConstants.mHeight = inHeight;
    thePushConstants.mColorA = glm::vec4( inA, 1.0f );
    thePushConstants.mColorB = glm::vec4( inB, 1.0f );
    thePushConstants.mColorC = glm::vec4( inC, 1.0f );
    thePushConstants.mColorD = glm::vec4( inD, 1.0f );
    thePushConstants.mTScale = inTScale;

    mImpl->mPipeline->PushConstants( inCommandBuffer, sizeof( Impl::PushConstantUniformObject ), &thePushConstants );
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eCompute, mImpl->mPipeline->GetVkPipeline(), {} );
    int workgroupSize = 16;
    inCommandBuffer.dispatch( ceil( (float) inWidth / workgroupSize ), ceil( (float) inHeight / workgroupSize ), 1 );

}

void
vkrt::graphics::CrystalViewShader::BindImage( vk::ImageView inOutputImageView )
{
    vks::DescriptorBinder theBinder( mImpl->mDevice, mImpl->mDescriptorSet->GetVkDescriptorSet() );

    vk::DescriptorImageInfo theImageInfo =
    {
        vk::Sampler(),
        inOutputImageView,
        vk::ImageLayout::eGeneral
    };
    theBinder.AddImageBinding( 0, vk::DescriptorType::eStorageImage, theImageInfo);

    theBinder.Bind();

}
