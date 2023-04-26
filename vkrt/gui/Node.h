//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_NODE_H
#define VKRT_NODE_H

#include "NodeContext.h"
#include "ForwardDecl.h"

namespace vkrt::gui
{
    class Node
    {
        public:
            enum Type
            {
                ImageGen,
                ImageOutput
            };

        public:
            Node( vkrt::gui::NodeContextPtr inContext, Type inType );
            ~Node();

        public:
            virtual void Draw() = 0;
            std::size_t GetId() const { return mId; }
            Type GetType() const { return mType; }

        private:
            std::size_t mId;
            Type mType;
    };

}

#endif //VKRT_NODE_H
