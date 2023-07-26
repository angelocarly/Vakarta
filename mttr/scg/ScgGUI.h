//
// Created by magnias on 7/21/23.
//

#ifndef SCG_GUI_H
#define SCG_GUI_H

#include "vkrt/gui/GuiDrawer.h"
#include "vks/core/Image.h"
#include "Presenter.h"

#include <string>

namespace scg
{
    class ScgGUI
    :
        public vkrt::gui::GuiDrawer
    {
        public:
            ScgGUI( vkrt::InputStatePtr inInputState, std::shared_ptr< scg::Presenter > inPresenter );
            ~ScgGUI() = default;

        public:
            void DrawGui();

        private:
            std::shared_ptr< scg::Presenter > mPresenter;
            vkrt::InputStatePtr mInputState;

            std::string mFilePath;
            std::string mFileName;
            vks::Image mImage;

    };
}

#endif //VKRT_IMPGUI_H
