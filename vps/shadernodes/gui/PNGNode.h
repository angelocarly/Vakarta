/**
 * PNGNode.h
 *
 * @file	PNGNode.h
 * @author	Angelo Carly
 * @date	29/04/2023
 *
 *
 */

#ifndef VKRT_PNGNODE_H
#define VKRT_PNGNODE_H

#include "Node.h"
#include "vks/core/Image.h"
#include "OutputAttribute.h"

namespace vkrt::gui
{
    class PNGNode
    :
        public vkrt::gui::Node
    {
        public:
            PNGNode( vkrt::gui::NodeContextPtr inContext );
            ~PNGNode();

            void Draw() override;
            vks::Image GetImage();

        private:
            std::shared_ptr< vkrt::gui::OutputAttribute< vks::Image > > mOutput;

            std::string mFileName;
            std::string mFilePath;
            vks::Image mImage;
    };
}


#endif //VKRT_PNGNODE_H
