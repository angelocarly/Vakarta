/**
 * ForwardDecl.h
 *
 * @file	ForwardDecl.h
 * @author	Angelo Carly
 * @date	24/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Vkrt_Gui_ForwardDecl_h
#define Vkrt_Gui_ForwardDecl_h

#include <memory>

namespace vkrt::gui
{
    class GuiDrawer;
    using GuiDrawerPtr = std::shared_ptr< GuiDrawer >;
}

#endif
