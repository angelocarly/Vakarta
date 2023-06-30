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
    :
    public vkrt::Drawable
    {

        public:
            Engine( vks::WindowPtr inWindow, vks::VksSessionPtr inVulkanSession );
            ~Engine();

            void Run();
            void RegisterGuiDrawer( std::weak_ptr< gui::GuiDrawer > inGuiDrawer );
            void RegisterPresenter( std::shared_ptr< Presentable > inPresenter );
            void AddDrawable( std::shared_ptr< Drawable > inDrawable );

            InputStatePtr GetInputState() const { return mInputState; }
            vks::VksSessionPtr GetVulkanSession() const { return mVulkanSession; }

        protected:
            virtual void Update( float inFrameDuration ) = 0;

        private:
            void UpdateSelf( float inFrameDuration );
            void RenderSelf();
            bool ShouldClose();

        private:
            vks::WindowPtr mWindow;
            vks::VksSessionPtr mVulkanSession;
            vkrt::Display mRenderer;
            vkrt::InputStatePtr mInputState;
            std::chrono::microseconds mPreviousSecond;

            std::vector< std::shared_ptr< Drawable > > mDrawables;

        private:
            // Resources
            vks::AssetLoader mAssetLoader;

            // Gui
            vks::ImGuiPtr mGui;
    };
}

#endif //VKRT_ENGINE_H
