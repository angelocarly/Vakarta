//
// Created by magnias on 1/29/23.
//

#ifndef VKRT_GUILAYER_H
#define VKRT_GUILAYER_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include "vkrt/graphics/Presenter.h"
#include "vkrt/graphics/Renderer.h"
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
            GuiLayer( InputStatePtr inInputState );
            ~GuiLayer();

            void Begin();
            void Stats( vkrt::GuiLayer::StatsInfo & inStats );
            void Tools( vkrt::Renderer & inRenderer );
            void ImageTest( vkrt::Presenter & inPresenter );
            void NodesTest( vkrt::Presenter & inPresenter );
            void End();

        public:
            bool mShowStats = true;
            bool mShowTools = true;
            bool mShowDemoWindow = false;

        private:
            GuiNodes mGuiNodes;
    };
}


#endif //VKRT_GUILAYER_H
