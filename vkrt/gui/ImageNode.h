//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_IMAGENODE_H
#define VKRT_IMAGENODE_H

#include "Node.h"
#include "ForwardDecl.h"
#include "ImageProvider.h"

#include <cstdio>

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
            void SetImageProvider( std::shared_ptr< vkrt::gui::ImageProvider > inImageProvider );

        private:
            std::size_t mInputId;
            std::size_t mOutputId;

            std::shared_ptr< vkrt::gui::ImageProvider > mImageProvider;
    };
}

#endif //VKRT_IMAGENODE_H
