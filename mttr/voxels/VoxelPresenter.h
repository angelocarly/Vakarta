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
#include "VoxelControls.h"

#include <glm/vec4.hpp>

namespace Mttr::Vox
{
    class VoxelPresenter
    :
        public vkrt::Presenter
    {
        public:
            VoxelPresenter( vks::DevicePtr inDevice, std::size_t inWidth, std::size_t inHeight );
            ~VoxelPresenter();

            void SetVoxelControls( std::shared_ptr< Mttr::Vox::VoxelControls > inVoxelControls );
            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment ) override;

        private:
            void InitializePipeline( vk::RenderPass const inRenderPass );

        private:
            vks::DevicePtr mDevice;
            vks::PipelinePtr mPipeline;
            std::size_t mWidth;
            std::size_t mHeight;
            std::shared_ptr< VoxelControls > mVoxelControls;

        private:
            struct PushConstants
            {
                std::uint32_t mScreenWidth;
                std::uint32_t mScreenHeight;
                std::uint32_t mPadding1;
                std::uint32_t mPadding2;
                glm::vec4 mCameraPosition;
            };
    };
}

#endif
