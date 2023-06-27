//
// Created by magnias on 7/11/22.
//

#ifndef VKRT_ENGINE_H
#define VKRT_ENGINE_H

#include <GLFW/glfw3.h>

#include "vks/render/Instance.h"
#include "vks/render/PhysicalDevice.h"

#include "vkrt/core/ForwardDecl.h"
#include "vkrt/graphics/Display.h"
#include "vkrt/gui/ForwardDecl.h"

namespace vkrt
{
    class Engine
    {

        public:
            Engine( vks::WindowPtr inWindow, vks::VulkanSessionPtr inVulkanSession, vkrt::PresenterPtr inPresenter );
            ~Engine();

            void Run();
            void RegisterGuiDrawer( std::weak_ptr< gui::GuiDrawer > inGuiDrawer );

            InputStatePtr GetInputState() const { return mInputState; }
            vks::VulkanSessionPtr GetVulkanSession() const { return mVulkanSession; }

        private:
            void Update( float inFrameDuration );
            void Render();
            bool ShouldClose();

        private:
            vks::WindowPtr mWindow;
            vks::VulkanSessionPtr mVulkanSession;
            vkrt::Display mRenderer;
            vkrt::InputStatePtr mInputState;
            std::chrono::microseconds mPreviousSecond;

        private:
            // Resources
            vks::AssetLoader mAssetLoader;
            std::unique_ptr< vks::Mesh > mMesh;

            // Gui
            vks::ImGuiPtr mGui;
    };
}

#endif //VKRT_ENGINE_H
