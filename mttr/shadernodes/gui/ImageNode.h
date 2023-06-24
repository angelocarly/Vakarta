//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_IMAGENODE_H
#define VKRT_IMAGENODE_H

#include "Node.h"
#include "ForwardDecl.h"
#include "ImageProvider.h"
#include "vks/core/Image.h"

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

        private:
            void SetupImageViews( std::optional< vks::Image > inImage );
            void DisconnectImageViews();

            std::shared_ptr< vkrt::gui::InputAttribute< vks::Image > > mInput;

            vk::ImageView mImageView;
            vk::Sampler mSampler;
            vk::DescriptorSet mDescriptorSet;
    };
}

#endif //VKRT_IMAGENODE_H
