//
// Created by magnias on 7/21/23.
//

#ifndef VKRT_IMPGUI_H
#define VKRT_IMPGUI_H

#include "vkrt/gui/GuiDrawer.h"
#include "vks/core/Image.h"
#include "Presenter.h"

#include <string>

namespace Mttr::Imp
{
    class ImpGUI
    :
        public vkrt::gui::GuiDrawer
    {
        public:
            ImpGUI( vkrt::InputStatePtr inInputState, std::shared_ptr< Mttr::Imp::Presenter > inPresenter );
            ~ImpGUI() = default;

        public:
            void DrawGui();

        private:
            std::shared_ptr< Mttr::Imp::Presenter > mPresenter;
            vkrt::InputStatePtr mInputState;

            std::string mFilePath;
            std::string mFileName;
            vks::Image mImage;

    };
}

#endif //VKRT_IMPGUI_H
