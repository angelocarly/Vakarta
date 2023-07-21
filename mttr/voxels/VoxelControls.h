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
            VoxelControls( vkrt::InputStatePtr inInputState );
            ~VoxelControls();

            glm::vec3 GetCameraPosition() const { return mCamera->GetPosition(); }
            glm::vec3 GetCameraDirection() const { return mCamera->GetForward(); }
            glm::mat4 GetModel() const;
            glm::mat4 GetView() const;
            glm::mat4 GetProjection() const;
            vkrt::CameraPtr GetCamera() const { return mCamera; }

            bool ShouldCompute() const { return mShouldCompute; };

        public:
            void DrawGui() override;

        private:
            vkrt::CameraPtr mCamera;
            vkrt::InputStatePtr mInputState;

            std::size_t mFrameCount = 0;
            bool mStepCompute = false;
            bool mShouldCompute = false;
            bool mComputeEnabled = false;
            std::uint32_t mFramesPerCompute = 20;
    };
}

#endif
