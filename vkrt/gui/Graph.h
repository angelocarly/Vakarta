//
// Created by magnias on 5/6/23.
//

#ifndef VKRT_GUI_GRAPH_H
#define VKRT_GUI_GRAPH_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <cstddef>
#include <cassert>
#include <algorithm>

namespace vkrt::gui
{
    template< typename ElementType >
    class IdMap
    {
        public:
            using iterator = typename std::unordered_map< std::size_t, ElementType >::iterator;
            using const_iterator = typename std::unordered_map< std::size_t, ElementType >::const_iterator;

            const_iterator begin() const { return mMap.begin(); }
            const_iterator end() const { return mMap.end(); }

            IdMap() {}
            ~IdMap() {}

            std::size_t insert( ElementType inElement );
            bool empty() { return mMap.empty(); };
            std::size_t size() { return mMap.size(); };
            bool contains( const std::size_t inId );
            ElementType & operator[]( const std::size_t inId );
            void remove( const std::size_t inId );
            size_t insert( const size_t inId, ElementType && inElement );
            size_t insert( const size_t inId, ElementType & inElement );

            std::unordered_map< std::size_t, ElementType > elements() { return mMap; }

            ElementType
            get( const std::size_t inId )
            {
                assert( mMap.contains( inId ) );
                return mMap[ inId ];
            }

            size_t Insert( size_t inId, ElementType inElement );

        private:
            std::unordered_map< std::size_t, ElementType > mMap;
            std::vector< std::size_t > mFreeIds;

            std::size_t RetrieveNextFreeId()
            {
                if( mFreeIds.empty() )
                {
                    int theId = 0;
                    while( mMap.contains( theId ) )
                    {
                        theId++;
                    }
                    return theId;
                }
                else
                {
                    auto theId = mFreeIds.back();
                    mFreeIds.pop_back();
                    return theId;
                }
            }

    };

    class Edge
    {
        public:
            std::size_t mSrcNodeId;
            std::size_t mDstNodeId;
    };

    template< typename ElementType >
    std::size_t
    IdMap< ElementType >::insert( ElementType inElement )
    {
        auto theId = RetrieveNextFreeId();
        Insert( theId, inElement );
        return theId;
    }

    template< typename ElementType >
    std::size_t
    IdMap< ElementType >::Insert( std::size_t inId, ElementType inElement )
    {
        assert( !mMap.contains( inId ) );
        mMap.insert( std::make_pair( inId, inElement ) );
        return inId;
    }

    template< typename ElementType >
    std::size_t IdMap< ElementType >::insert( const std::size_t inId, ElementType & inElement )
    {
        assert( !mMap.contains( inId ) );
        mMap[ inId ] = inElement;
        return inId;
    }

    template< typename ElementType >
    void IdMap< ElementType >::remove( const std::size_t inId )
    {
        mMap.erase( inId );
    }

    template< typename ElementType >
    bool IdMap< ElementType >::contains( const std::size_t inId )
    {
        return mMap.contains( inId );
    }

    template< typename NodeType >
    class Graph
    {
        public:
            Graph();
            ~Graph();

            std::size_t addNode( NodeType inNode );
            void removeNode( const std::size_t inId );
            std::size_t addEdge( const std::size_t inSrcId, const std::size_t inDstId );
            void removeEdge( const std::size_t inId );
            std::unordered_map< std::size_t, Edge > GetEdges();
            NodeType GetNode( size_t inId );
            std::unordered_map< std::size_t, NodeType > GetNodes();
            std::vector< size_t > GetInputNodes( size_t inNodeId );

        private:
            IdMap< NodeType > mNodes;
            IdMap< Edge > mEdges;

    };

    template< typename NodeType >
    Graph< NodeType >::~Graph()
    {
        //  TODO: destroy shit
    }

    template< typename NodeType >
    std::unordered_map< std::size_t, NodeType >
    Graph< NodeType >::GetNodes()
    {
        return mNodes.elements();
    }

    template< typename NodeType >
    NodeType
    Graph< NodeType >::GetNode( std::size_t inId )
    {
        assert( mNodes.contains( inId ) );
        return mNodes.get( inId );
    }

    template< typename NodeType >
    std::vector< std::size_t >
    Graph< NodeType >::GetInputNodes( std::size_t inNodeId )
    {
        std::vector< std::size_t > mInputLinks;
        for( auto theLink : mEdges )
        {
            if( theLink.second.mDstNodeId == inNodeId )
            {
                mInputLinks.push_back( theLink.second.mSrcNodeId );
            }
        }

        return mInputLinks;
    }

    template< typename NodeType >
    std::unordered_map< std::size_t, Edge >
    vkrt::gui::Graph< NodeType >::GetEdges()
    {
        return mEdges.elements();
    }

    template< typename NodeType >
    vkrt::gui::Graph< NodeType >::Graph()
    {
    }

    template< typename NodeType >
    std::size_t
    vkrt::gui::Graph< NodeType >::addNode( NodeType inNode )
    {
        auto theId = mNodes.insert( inNode );
        return theId;
    }

    template< typename NodeType >
    void
    vkrt::gui::Graph< NodeType >::removeNode( const std::size_t inId )
    {
        assert( mNodes.contains( inId ) );
        mNodes.remove( inId );
    }

    template< typename NodeType >
    std::size_t
    vkrt::gui::Graph< NodeType >::addEdge( const std::size_t src, const std::size_t dst )
    {
        Edge theEdge;
        theEdge.mSrcNodeId = src;
        theEdge.mDstNodeId = dst;
        auto theId = mEdges.insert( theEdge );
        return theId;
    }

    template< typename NodeType >
    void vkrt::gui::Graph< NodeType >::removeEdge( const std::size_t inId )
    {
        assert( mEdges.contains( inId ) );
        mEdges.remove( inId );
    }

    template< typename NodeType, typename Visitor >
    void traverse_graph( Graph< NodeType > graph, Visitor visitor )
    {
        std::vector< std::size_t > theIdStack;
        for( auto theId : graph.GetNodes() )
        {
            theIdStack.push_back( theId.first );
        }

        std::vector< std::size_t > theVisitedIds;
        while( !theIdStack.empty() )
        {
            std::size_t theId = theIdStack.front();
            theIdStack.erase( theIdStack.begin() );

            // Only process nodes we haven't visited yet
            if( std::find( theVisitedIds.begin(), theVisitedIds.end(), theId ) != theVisitedIds.end() )
            {
                continue;
            }

            auto theNode = graph.GetNode( theId );

            auto theInputNodes = graph.GetInputNodes( theId );
            if( theInputNodes.empty() )
            {
                // Start nodes may always be processed
                visitor( theNode );
                theVisitedIds.push_back( theId );
            }
            else
            {
                // Nodes with inputs may only be processed if all inputs have been processed
                bool theAllInputsVisited = true;
                for( auto theInputId : theInputNodes )
                {
                    if( std::find( theVisitedIds.begin(), theVisitedIds.end(), theInputId ) == theVisitedIds.end() )
                    {
                        theAllInputsVisited = false;
                        theIdStack.insert( theIdStack.begin(), theInputId );
                    }
                }

                if( theAllInputsVisited )
                {
                    visitor( theNode );
                    theVisitedIds.push_back( theId );
                }
                else
                {
                    theIdStack.push_back( theId );
                }
            }

        }
    }

}
#endif //VKRT_GRAPH_H
