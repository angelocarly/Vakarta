/**
 * GuiNodes.h
 *
 * @file	GuiNodes.h
 * @author	Angelo Carly
 * @date	22/04/2023
 *
 *
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

            void Update();
            void Draw( vkrt::Presenter & inPresenter );

        private:
            InputStatePtr mInputState;

            vkrt::gui::NodeContextPtr mNodeContext;
    };
}


#endif //VKRT_GUINODES_H
