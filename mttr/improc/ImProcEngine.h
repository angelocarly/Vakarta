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
#include "mttr/improc/Presenter.h"
#include "mttr/improc/ImpGUI.h"

namespace Mttr::Imp
{
    class ImProcEngine
    :
        public vkrt::Engine
    {
        public:
            ImProcEngine( vks::WindowPtr inWindow, vks::VksSessionPtr inSession );
            ~ImProcEngine();

            void Update( float inFrameDuration );
            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment );

            vk::ImageView GetImageView() const { return mPresenter->GetImageView(); };
            vk::Sampler GetSampler() const { return mPresenter->GetSampler(); };

        private:
            std::shared_ptr< Mttr::Imp::Presenter > mPresenter;
            std::shared_ptr< Mttr::Imp::ImpGUI > mGui;
            vkrt::CameraPtr mCamera;
    };
}

#endif //VKRT_VOXELENGINE_H
