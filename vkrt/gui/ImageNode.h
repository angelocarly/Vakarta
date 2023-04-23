//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_IMAGENODE_H
#define VKRT_IMAGENODE_H

#include <cstdio>
#include "Node.h"
#include "ForwardDecl.h"

namespace vkrt::gui
{
    class ImageNode
    :
        public vkrt::gui::Node
    {
        public:
            ImageNode( vkrt::gui::NodeContextPtr inContext );
            ~ImageNode();

        public:
            void Draw() override;

        private:
            std::size_t mInputId;
            std::size_t mOutputId;
    };
}

#endif //VKRT_IMAGENODE_H
