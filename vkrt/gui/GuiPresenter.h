//
// Created by magnias on 6/18/23.
//

#ifndef VKRT_GUIPRESENTER_H
#define VKRT_GUIPRESENTER_H

#include "../graphics/Presenter.h"

#include <memory>

namespace vkrt
{
    class GuiPresenter
    :
        public Presenter
    {
        private:

        public:
            static std::shared_ptr< GuiPresenter > GetInstance();
            static void Initialize( vks::DevicePtr inDevice, vks::SwapchainPtr inSwapchain, vks::WindowPtr inWindow );
            ~GuiPresenter();

            void Draw( vkrt::RenderEnvironment const & inRenderEnvironment ) override;

        private:
            GuiPresenter( vks::DevicePtr inDevice, vks::SwapchainPtr inSwapchain, vks::WindowPtr inWindow );

            void InitializeDescriptorPool();
            void InitializeImGui();
            void SetImGuiStyle();
            static void CheckVkResult( VkResult inResult );

        private:
            vks::DevicePtr mDevice;
            vk::DescriptorPool mDescriptorPool;
            vks::SwapchainPtr mSwapChain;
            vks::WindowPtr mWindow;

    };
}

#endif //VKRT_GUIPRESENTER_H
