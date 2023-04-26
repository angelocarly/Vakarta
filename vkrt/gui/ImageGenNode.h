//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_IMAGEGENNODE_H
#define VKRT_IMAGEGENNODE_H

#include "Node.h"
#include "ShaderEditor.h"
#include "ImageProvider.h"
#include "vkrt/graphics/ReflectionPresenter.h"

namespace vkrt::gui
{
    class ImageGenNode
    :
        public vkrt::gui::Node,
        public vkrt::gui::ImageProvider
    {
        public:
            ImageGenNode( vkrt::gui::NodeContextPtr inContext );
            ~ImageGenNode();

        public:
            void Draw() override;
            vk::DescriptorSet ProvideImage() override;

        private:
            std::size_t mOutputId;
            bool mShowEditor = false;
            vkrt::gui::ShaderEditor mEditor;

            vkrt::ReflectionPresenter mReflectionPresenter;
    };
}

#endif //VKRT_IMAGEGENNODE_H
