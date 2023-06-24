//
// Created by magnias on 1/29/23.
//

#ifndef VKRT_GUILAYER_H
#define VKRT_GUILAYER_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include "vkrt/graphics/Presenter.h"
#include "vkrt/graphics/Display.h"
#include "GuiNodes.h"

namespace vkrt
{
    class GuiLayer
    {
        public:
            struct StatsInfo
            {
                float mFrameTime;
                float mFps;
            };

        public:
            GuiLayer( vkrt::InputStatePtr inInputState, ImGuiContext * inContext );
            ~GuiLayer();

            void Begin();
            void Stats( vkrt::GuiLayer::StatsInfo & inStats );
            void Tools( vkrt::Display & inRenderer );
            void ImageTest( vkrt::Presenter & inPresenter );
            void NodesTest( vkrt::Presenter & inPresenter );
            void End();

        public:
            bool mShowStats = true;
            bool mShowTools = true;
            bool mShowDemoWindow = false;

        private:
            std::shared_ptr< GuiNodes > mGuiNodes;
    };
}


#endif //VKRT_GUILAYER_H
