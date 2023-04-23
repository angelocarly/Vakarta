//
// Created by magnias on 1/29/23.
//

#ifndef VKRT_GUIPANELS_H
#define VKRT_GUIPANELS_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Engine.h"
#include "vkrt/graphics/Presenter.h"
#include "GuiNodes.h"

namespace vkrt
{
    class GuiPanels
    {
        public:
            static void Begin();
            static void Stats( vkrt::Engine::Stats & inStats );
            static void Tools( vkrt::Renderer & inRenderer );
            static void ImageTest( vkrt::Presenter & inPresenter );
            static void NodesTest();
            static void End();

        public:
            inline static bool mShowStats = true;
            inline static bool mShowTools = true;
            inline static bool mShowDemoWindow = false;

        private:
            inline static GuiNodes mGuiNodes = vkrt::GuiNodes();
    };
}


#endif //VKRT_GUIPANELS_H
