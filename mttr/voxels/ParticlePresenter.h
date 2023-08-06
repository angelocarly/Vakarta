/**
 * ParticlePresenter.h
 *
 * @file	ParticlePresenter.h
 * @author	Angelo Carly
 * @date	29/07/2023
 *
 */

#ifndef Mttr_VoxelPresenter_h
#define Mttr_VoxelPresenter_h

#include "vkrt/graphics/Presenter.h"

#include "mttr/voxels/VoxelControls.h"
#include "VoxelCompute.h"

#include <glm/vec4.hpp>

namespace Vox
{
    class ParticlePresenter
    :
        public vkrt::Presenter
    {
        public:
            ParticlePresenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera, std::size_t inWorldSize );
            ~ParticlePresenter();

            void Draw( const vkrt::RenderEnvironment & inRenderEnvironment) override;
            vks::Buffer GetWorldBuffer() { return mWorldBuffer; };

        private:
            void InitializeDescriptorSetLayout();
            void InitializeDisplayPipeline( vk::RenderPass const inRenderPass );
            void InitializeBuffers();

        private:
            vks::DevicePtr mDevice;
            vks::PipelinePtr mPipeline;
            std::size_t mWidth;
            std::size_t mHeight;
            vk::DescriptorSetLayout mDescriptorSetLayout;

            vks::Buffer mWorldBuffer;

            vkrt::CameraPtr mCamera;

            std::size_t const kWorldSize;

        private:
            struct PushConstants
            {
                glm::mat4 mMVP;
                glm::mat4 mViewProjection;
            };
    };
}

#endif
