//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_NODECONTEXT_H
#define VKRT_NODECONTEXT_H

#include <utility>
#include <vector>
#include <map>
#include "IdCache.h"
#include "ForwardDecl.h"

namespace vkrt::gui
{

    class NodeContext
    {
        public:
            struct Link
            {
                std::size_t mA;
                std::size_t mB;
            };

            NodeContext();
            ~NodeContext();

            std::size_t GetNextLinkId() { return mLinks.size(); }
            std::size_t AddNode( Node * inNode );
            NodePtr GetNode( std::size_t inIndex ) { return mNodes[ inIndex ]; };
            std::size_t AddAttribute();
            std::size_t AddLink( std::size_t a, std::size_t b );
            [[nodiscard]] std::map< std::size_t, Link > GetLinks() const { return mLinks; }
            [[nodiscard]] std::map< std::size_t, NodePtr > GetNodes() const { return mNodes; }
            void RemoveLink( std::size_t inId );

        private:

            std::map< std::size_t, Link > mLinks;
            std::map< std::size_t, NodePtr > mNodes;
            vkrt::gui::IdCache mLinkIdCache;
            vkrt::gui::IdCache mNodeIdCache;
            vkrt::gui::IdCache mAttributes;
    };

}

#endif //VKRT_NODECONTEXT_H
