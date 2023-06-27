//
// Created by magnias on 6/18/23.
//

#ifndef VKRT_GUIPRESENTER_H
#define VKRT_GUIPRESENTER_H

#include "../graphics/Presenter.h"
#include "ForwardDecl.h"

#include "imgui.h"

#include <memory>

namespace vkrt
{
    class GuiPresenter
    :
        public Presenter
    {
        public:
            static std::shared_ptr< GuiPresenter > GetInstance();
            static void Initialize( vks::DevicePtr inDevice, vks::SwapchainPtr inSwapchain, vks::WindowPtr inWindow );
            ~GuiPresenter();

            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment ) override;
            void RegisterGuiDrawer( std::weak_ptr< vkrt::gui::GuiDrawer > inGuiDrawer );

            static void Finalize();

        private:
            GuiPresenter( vks::DevicePtr inDevice, vks::SwapchainPtr inSwapchain, vks::WindowPtr inWindow );

            void InitializeDescriptorPool();
            void InitializeImGui();
            void SetImGuiStyle( ImGuiIO & io );
            static void CheckVkResult( VkResult inResult );

        private:
            vks::DevicePtr mDevice;
            vk::DescriptorPool mDescriptorPool;
            vks::SwapchainPtr mSwapChain;
            vks::WindowPtr mWindow;

            std::vector< std::weak_ptr< vkrt::gui::GuiDrawer > > mGuiDrawers;

    };
}

#endif //VKRT_GUIPRESENTER_H
