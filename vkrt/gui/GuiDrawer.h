/**
 * GuiDrawer.h
 *
 * @file	GuiDrawer.h
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Vkrt_GuiDrawer_h
#define Vkrt_GuiDrawer_h

namespace vkrt::gui
{
    class GuiDrawer
    {
        public:
            virtual void DrawGui() = 0;
    };
}

#endif
