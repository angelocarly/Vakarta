/**
 * VoxelPresenter.h
 *
 * @file	VoxelPresenter.h
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#ifndef Mttr_VoxelPresenter_h
#define Mttr_VoxelPresenter_h

#include "vkrt/graphics/Presenter.h"

#include "mttr/voxels/VoxelControls.h"

#include <glm/vec4.hpp>

namespace Mttr::Imp
{
    class Presenter
    :
        public vkrt::Presenter
    {
        public:
            Presenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera );
            ~Presenter();

            void Draw( const vkrt::RenderEnvironment & inRenderEnvironment) override;
            void SetImage( vks::Image & inImage );
            glm::vec3 & GetColorTreshold() { return mColorTreshold; };
            glm::vec3 & GetReplacementColor() { return mReplacementColor; };
            glm::vec2 & GetPos() { return mPos; };
            float & GetScale() { return mScale; };

        private:
            void InitializeDescriptorSetLayout();
            void InitializeDisplayPipeline( vk::RenderPass inRenderPass );
            void InitializeBuffers();
            void SetupImageViews( vks::Image & inImage );

        private:
            vks::DevicePtr mDevice;
            vks::PipelinePtr mPipeline;
            std::size_t mWidth;
            std::size_t mHeight;
            vk::DescriptorSetLayout mDescriptorSetLayout;

            vkrt::CameraPtr mCamera;

        private:
            struct PushConstants
            {
                glm::vec2 mSize;
                glm::vec2 mPos;
                glm::vec4 mColorTreshold;
                glm::vec4 mReplacementColor;
                float mScale;
            };

            vk::ImageView mImageView;
            vk::Sampler mSampler;
            bool mCanRender = false;

            glm::vec3 mColorTreshold;
            glm::vec3 mReplacementColor;

            glm::vec2 mPos;
            glm::vec2 mSize;
            float mScale = 1.0f;
    };
}

#endif
