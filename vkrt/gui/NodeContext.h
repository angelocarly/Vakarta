//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_NODECONTEXT_H
#define VKRT_NODECONTEXT_H

#include <utility>
#include <vector>
#include <map>
#include "IdCache.h"

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
            std::size_t AddNode();
            std::size_t AddAttribute();
            std::size_t AddLink( std::size_t a, std::size_t b );
            [[nodiscard]] std::map< std::size_t, Link > GetLinks() const { return mLinks; }
            void RemoveLink( std::size_t inId );

        private:

            std::map< std::size_t, Link > mLinks;
            vkrt::gui::IdCache mLinkIdCache;
            vkrt::gui::IdCache mNodes;
            vkrt::gui::IdCache mAttributes;
    };

}

#endif //VKRT_NODECONTEXT_H
