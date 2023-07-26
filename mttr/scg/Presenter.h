/**
 * VoxelPresenter.h
 *
 * @file	VoxelPresenter.h
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#ifndef Scg_ScgPresenter_h
#define Scg_ScgPresenter_h

#include "vkrt/graphics/Presenter.h"

#include "mttr/voxels/VoxelControls.h"

#include <glm/vec4.hpp>

namespace scg
{
    class Presenter
    :
        public vkrt::Presenter
    {
        public:
            Presenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera );
            ~Presenter();

            void Draw( const vkrt::RenderEnvironment & inRenderEnvironment) override;
            glm::vec4 & GetColorTreshold() { return mPushConstants.mColorBackGround; };
            glm::vec4 & GetReplacementColor() { return mPushConstants.mReplacementColor; };
            glm::vec2 & GetPos() { return mPushConstants.mPos; };
            float * GetSymBuffer() { return mSymBuffer; };
            float & GetEdgeSize() { return mPushConstants.mEdgeSize; };
            float & GetScale() { return mPushConstants.mScale; };

        private:
            void InitializeDescriptorSetLayout();
            void InitializeDisplayPipeline( vk::RenderPass inRenderPass );
            void InitializeBuffers();

        private:
            vks::DevicePtr mDevice;
            vks::PipelinePtr mPipeline;
            std::size_t mWidth;
            std::size_t mHeight;
            vk::DescriptorSetLayout mDescriptorSetLayout;

            vkrt::CameraPtr mCamera;
            vks::Buffer mGeometryBuffer;

        private:
            struct PushConstants
            {
                glm::vec2 mSize;
                glm::vec2 mPos;
                glm::vec4 mColorBackGround;
                glm::vec4 mReplacementColor = glm::vec4( 1.0f );
                float mScale = 1.0f;
                float mEdgeSize = 0.01f;
            };
            PushConstants mPushConstants;
            float * mSymBuffer;
    };
}

#endif
