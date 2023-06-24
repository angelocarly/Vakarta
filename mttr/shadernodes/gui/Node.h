//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_NODE_H
#define VKRT_NODE_H

#include "NodeContext.h"
#include "InputAttribute.h"

namespace vkrt::gui
{
    class Node
    {
        public:
            Node( std::size_t inId = -1 );
            ~Node();

        public:
            virtual void Draw() = 0;
            std::size_t GetId() const { return mId; }
            void SetId( const std::size_t inId )
            {
                mId = inId;
                // Update all attributes
                for( auto theAttribute : mAttributes )
                {
                    theAttribute->mNodeId = inId;
                }
            }

            void LinkAttribute( std::shared_ptr< GuiAttribute > inAttribute )
            {
                inAttribute->mNodeId = mId;
                mAttributes.push_back( inAttribute );
            }

        private:
            std::size_t mId;
            std::vector< std::shared_ptr< GuiAttribute > > mAttributes;
    };
}

#endif //VKRT_NODE_H
