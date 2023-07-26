/**
 * VoxelEngine.h
 *
 * @file	VoxelEngine.h
 * @author	Angelo Carly
 * @date	29/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Scg_ScgEngine_h
#define Scg_ScgEngine_h

#include "vkrt/application/Engine.h"
#include "mttr/scg/Presenter.h"
#include "mttr/scg/ScgGUI.h"

namespace scg
{
    class ScgEngine
    :
        public vkrt::Engine
    {
        public:
            ScgEngine( vks::WindowPtr inWindow, vks::VksSessionPtr inSession );
            ~ScgEngine();

            void Update( float inFrameDuration );
            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment );

            vk::ImageView GetImageView() const { return mPresenter->GetImageView(); };
            vk::Sampler GetSampler() const { return mPresenter->GetSampler(); };

        private:
            std::shared_ptr< scg::Presenter > mPresenter;
            std::shared_ptr< scg::ScgGUI > mGui;
            vkrt::CameraPtr mCamera;
    };
}

#endif //VKRT_VOXELENGINE_H
