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

#include <glm/vec3.hpp>

namespace Mttr::Vox
{
    class VoxelControls
    :
        public vkrt::gui::GuiDrawer
    {
        public:
            VoxelControls();
            ~VoxelControls();

            glm::vec3 GetCameraPosition() const { return mCameraPosition; }

        public:
            void DrawGui() override;

        private:
            glm::vec3 mCameraPosition;
    };
}

#endif
