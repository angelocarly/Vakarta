//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_SHADEREDITOR_H
#define VKRT_SHADEREDITOR_H

#include <memory>

namespace vkrt::gui
{
    class ShaderEditor
    {
        public:
            ShaderEditor();
            ~ShaderEditor();

        public:
            void Draw( bool & inOpen );

        private:
            struct ZepWrapper;

        private:
            std::shared_ptr<ZepWrapper> zepWrapper;
            bool z_init = false;
    };
}

#endif //VKRT_SHADEREDITOR_H
