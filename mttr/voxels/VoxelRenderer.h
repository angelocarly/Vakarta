/**
 * VoxelRenderer.h
 *
 * @file	VoxelRenderer.h
 * @author	Angelo Carly
 * @date	30/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Mttr_VoxelRenderer_h
#define Mttr_VoxelRenderer_h

#include "vkrt/graphics/Presentable.h"
#include "vkrt/graphics/RenderEnvironment.h"
#include "VoxelPresenter.h"

namespace Mttr::Vox
{
    class VoxelRenderer
    :
        public vkrt::Presentable
    {
        private:

        public:
            VoxelRenderer( vks::DevicePtr inDevice, std::shared_ptr< VoxelControls > inVoxelControls, std::size_t inWidth, std::size_t inHeight, vkrt::CameraPtr inCamera );
            ~VoxelRenderer();

            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment );
            vk::ImageView GetImageView() const { return mVoxelPresenter->GetImageView(); };
            vk::Sampler GetSampler() const { return mVoxelPresenter->GetSampler(); };

        private:
            std::shared_ptr< Mttr::Vox::VoxelPresenter > mVoxelPresenter;
            std::shared_ptr< Mttr::Vox::VoxelCompute > mVoxelCompute;
            std::shared_ptr< Mttr::Vox::VoxelControls > mVoxelControls;

    };
}

#endif
