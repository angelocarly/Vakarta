//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_IMAGEGENNODE_H
#define VKRT_IMAGEGENNODE_H

#include "Node.h"
#include "ShaderEditor.h"

namespace vkrt::gui
{
    class ImageGenNode
    :
        public vkrt::gui::Node
    {
        public:
            ImageGenNode( vkrt::gui::NodeContextPtr inContext );
            ~ImageGenNode();

        public:
            void Draw() override;

        private:
            std::size_t mOutputId;
            bool mShowEditor = false;
            vkrt::gui::ShaderEditor mEditor;
    };
}

#endif //VKRT_IMAGEGENNODE_H
