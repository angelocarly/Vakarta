//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_NODECONTEXT_H
#define VKRT_NODECONTEXT_H

#include "IdCache.h"
#include "ForwardDecl.h"
#include "Graph.h"
#include "InputAttribute.h"
#include "OutputAttribute.h"

#include <vulkan/vulkan.hpp>

#include <utility>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <optional>

namespace vkrt::gui
{

    class NodeContext
    {
        public:
            struct Link
            {
                std::size_t mId;
                std::size_t mSrcAttribute;
                std::size_t mDstAttribute;
            };

        public:
            NodeContext();
            ~NodeContext();

            void AddNode( std::shared_ptr< Node > inNode );
            void AddAttribute( std::shared_ptr< GuiAttribute > inAttribute );
            void AddLink( const std::size_t inSrc, const std::size_t inDst );
            void RemoveLink( const std::size_t inId );

            std::vector< vkrt::gui::NodeContext::Link > GetLinks();

            template< typename Visitor >
            void Traverse( Visitor visitor )
            {
                gui::traverse_graph( mGraph, visitor );
            }

            template< typename T >
            std::shared_ptr< vkrt::gui::InputAttribute< T > >
            CreateInputAttribute( std::function< void( std::optional< T > ) > inConnectCallback, std::function< void( std::optional< T > ) > inUpdateCallback, std::function< void( void ) > inDisconnectCallback )
            {
                auto theAttribute = std::make_shared< InputAttribute< T > >( inConnectCallback, inUpdateCallback, inDisconnectCallback );
                AddAttribute( theAttribute );
                return theAttribute;
            }

            template< typename T >
            std::shared_ptr< vkrt::gui::OutputAttribute< T > >
            CreateOutputAttribute( std::function< std::optional< T >( void ) > inCallback )
            {
                auto theAttribute = std::make_shared< OutputAttribute< T > >( inCallback );
                AddAttribute( theAttribute );
                return theAttribute;
            }

        private:
            Graph< std::shared_ptr< Node > > mGraph;
            std::vector< std::shared_ptr< Node > > mNodes;
            IdMap< std::shared_ptr< GuiAttribute > > mAttributes;

            IdMap< Link > mLinks;
    };

}

#endif //VKRT_NODECONTEXT_H
