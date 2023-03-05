//
// Created by magnias on 1/29/23.
//

#ifndef VKRT_GUIPANELS_H
#define VKRT_GUIPANELS_H

#include "Engine.h"

namespace vkrt
{
    class GuiPanels
    {
        public:
            static void Begin();
            static void Stats( vkrt::Engine::Stats & inStats );
            static void Tools( vkrt::Renderer & inRenderer );
            static void End();

        public:
            inline static bool mShowStats = true;
            inline static bool mShowTools = true;
            inline static bool mShowDemoWindow = false;
    };
}


#endif //VKRT_GUIPANELS_H
