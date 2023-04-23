/**
 * GuiNodes.h
 *
 * @file	GuiNodes.h
 * @author	Angelo Carly
 * @date	22/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_GUINODES_H
#define VKRT_GUINODES_H

#include "vkrt/graphics/Presenter.h"
#include "vkrt/core/ForwardDecl.h"
#include "vkrt/gui/ForwardDecl.h"
#include "ForwardDecl.h"

namespace vkrt
{
    class GuiNodes
    {
        public:
            GuiNodes( InputStatePtr inInputState );
            ~GuiNodes();

            void Draw( vkrt::Presenter & inPresenter );

        private:
            InputStatePtr mInputState;

            vkrt::gui::NodeContextPtr mNodeContext;
            std::vector< vkrt::gui::NodePtr > mNodes;
    };
}


#endif //VKRT_GUINODES_H
