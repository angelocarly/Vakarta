/**
 * VoxelEngine.h
 *
 * @file	VoxelEngine.h
 * @author	Angelo Carly
 * @date	29/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Mttr_VoxelEngine_h
#define Mttr_VoxelEngine_h


#include "vkrt/application/Engine.h"

#include "VoxelControls.h"
#include "VoxelPresenter.h"
#include "VoxelRenderer.h"

namespace Mttr::Vox
{
    class VoxelEngine
    :
        public vkrt::Engine
    {
            // penis
        public:
            VoxelEngine( vks::WindowPtr inWindow, vks::VksSessionPtr inSession );
            ~VoxelEngine();

            void Update( float inFrameDuration );
            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment );

            vk::ImageView GetImageView() const { return mVoxelRenderer->GetImageView(); };
            vk::Sampler GetSampler() const { return mVoxelRenderer->GetSampler(); };

        private:
            std::shared_ptr< Mttr::Vox::VoxelControls > mVoxelGui;
            std::shared_ptr< Mttr::Vox::VoxelRenderer > mVoxelRenderer;
    };
}

#endif //VKRT_VOXELENGINE_H
