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
            Node( vkrt::gui::NodeContextPtr inContext );
            ~Node();

        public:
            virtual void Draw() = 0;
            std::size_t GetId() const { return mId; }

        private:
            std::size_t mId;
    };

}

#endif //VKRT_NODE_H
