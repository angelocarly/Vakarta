/**
 * VoxelControls.h
 *
 * @file	VoxelControls.h
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 */

#ifndef Mttr_VoxelControls_h
#define Mttr_VoxelControls_h

#include "vkrt/gui/GuiDrawer.h"

#include "vkrt/application/Engine.h"

#include "vkrt/core/Camera.h"
#include "vkrt/core/ForwardDecl.h"

#include <glm/vec3.hpp>
#include <glm/fwd.hpp>

namespace Mttr::Vox
{
    class VoxelControls
    :
        public vkrt::gui::GuiDrawer
    {
        public:
            VoxelControls( std::shared_ptr< vkrt::Engine > inEngine );
            ~VoxelControls();

            glm::vec3 GetCameraPosition() const { return mCamera->GetPosition(); }
            glm::vec3 GetCameraDirection() const { return mCamera->GetForward(); }
            glm::mat4 GetModel() const;
            glm::mat4 GetView() const;
            glm::mat4 GetProjection() const;

        public:
            void DrawGui() override;

        private:
            vkrt::CameraPtr mCamera;
            std::shared_ptr< vkrt::Engine > mEngine;
    };
}

#endif
