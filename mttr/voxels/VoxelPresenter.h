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
#include "VoxelCompute.h"

#include <glm/vec4.hpp>

namespace Mttr::Vox
{
    class VoxelPresenter
    :
        public vkrt::Presenter
    {
        public:
            VoxelPresenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera, std::size_t inWorldSize );
            ~VoxelPresenter();

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
                std::uint32_t mScreenWidth;
                std::uint32_t mScreenHeight;
                std::uint32_t mPadding1;
                std::uint32_t mPadding2;
                glm::mat4 mModel;
                glm::mat4 mView;
                glm::mat4 mProjection;
            };
    };
}

#endif
