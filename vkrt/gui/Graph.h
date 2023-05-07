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
            using iterator = typename std::vector< ElementType >::iterator;
            using const_iterator = typename std::vector< ElementType >::const_iterator;

            const_iterator begin() const { return mElements.begin(); }
            const_iterator end() const { return mElements.end(); }

            IdMap() {}
            ~IdMap() {}

            std::size_t insert( ElementType inElement );
            bool empty() { return mIds.empty(); };
            std::size_t size() { return mIds.size(); };
            bool contains( const std::size_t inId );
            ElementType & operator[]( const std::size_t inId );
            void remove( const std::size_t inId );
            size_t insert( const size_t inId, ElementType && inElement );
            size_t insert( const size_t inId, ElementType & inElement );

            std::vector< ElementType > elements() { return mElements; }
            std::vector< std::size_t > ids() { return mIds; }

            ElementType
            get( const std::size_t inId )
            {
                auto theIterator = std::lower_bound( mIds.begin(), mIds.end(), inId );
                auto theIndex = std::distance( mIds.begin(), theIterator );
                return mElements[theIndex];
            }

        private:
            std::vector< ElementType > mElements;
            std::vector< std::size_t > mIds;
            std::size_t mNextId = 0;
    };

    template< typename ElementType >
    ElementType & IdMap< ElementType >::operator[]( const std::size_t inId )
    {
        auto theIterator = std::lower_bound( mIds.begin(), mIds.end(), inId );
        assert( theIterator != mIds.end() );
        auto theIndex = std::distance( mIds.begin(), theIterator );
        return mElements[theIndex];
    }

    class Edge
    {
        public:
            std::size_t mId;
            std::size_t mSrcNodeId;
            std::size_t mDstNodeId;
    };

    template< typename ElementType >
    std::size_t
    IdMap< ElementType >::insert( ElementType inElement )
    {
        auto theId = mNextId++;
        insert( theId, std::move( inElement ) );
        return theId;
    }

    template< typename ElementType >
    std::size_t IdMap< ElementType >::insert( const std::size_t inId, ElementType & inElement )
    {
        assert( std::lower_bound( mIds.begin(), mIds.end(), inId ) == mIds.end() );
        mElements.push_back( inElement );
        mIds.push_back( inId );
        return inId;
    }

    template< typename ElementType >
    std::size_t IdMap< ElementType >::insert( const std::size_t inId, ElementType && inElement )
    {
        assert( std::lower_bound( mIds.begin(), mIds.end(), inId ) == mIds.end() );
        mElements.push_back( inElement );
        mIds.push_back( inId );
        return inId;
    }

    template< typename ElementType >
    void IdMap< ElementType >::remove( const std::size_t inId )
    {
        auto theElement = std::lower_bound( mIds.begin(), mIds.end(), inId );
        mIds.erase( theElement );
        auto theElementId = std::next( mElements.begin(), std::distance( mIds.begin(), theElement ) );
        mElements.erase( theElementId );
    }

    template< typename ElementType >
    bool IdMap< ElementType >::contains( const std::size_t inId )
    {
        return std::lower_bound( mIds.cbegin(), mIds.cend(), inId ) != mIds.end();
    }

    template< typename NodeType >
    class Graph
    {
        public:
            Graph();
            ~Graph();

            std::size_t addNode( NodeType inNode );
            void insertNode( const size_t id, NodeType & inNode );
            void removeNode( const std::size_t inId );
            std::size_t addEdge( const std::size_t inSrcId, const std::size_t inDstId );
            void insertEdge( const size_t id, const size_t src, const size_t dst );
            void removeEdge( const std::size_t inId );
            std::vector< Edge > GetEdges();
            NodeType GetNode( size_t inId );
            std::vector< NodeType > GetNodes();
            std::vector< std::size_t > GetNodeIds();
            std::vector< size_t > GetInputNodes( size_t inNodeId );

        private:
            IdMap< NodeType > mNodes;
            IdMap< Edge > mEdges;
            std::size_t mCurrentId;

    };

    template< typename NodeType >
    Graph< NodeType >::~Graph()
    {
        //  TODO: destroy shit
    }

    template< typename NodeType >
    std::vector< NodeType >
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
            if( theLink.mDstNodeId == inNodeId )
            {
                mInputLinks.push_back( theLink.mSrcNodeId );
            }
        }

        return mInputLinks;
    }

    template< typename NodeType >
    std::vector< std::size_t >
    Graph< NodeType >::GetNodeIds()
    {
        return mNodes.ids();
    }

    template< typename NodeType >
    std::vector< Edge >
    vkrt::gui::Graph< NodeType >::GetEdges()
    {
        return mEdges.elements();
    }

    template< typename NodeType >
    vkrt::gui::Graph< NodeType >::Graph()
        :
        mCurrentId( 0 )
    {
    }

    template< typename NodeType >
    std::size_t
    vkrt::gui::Graph< NodeType >::addNode( NodeType inNode )
    {
        const std::size_t theId = mCurrentId++;
        mNodes.insert( theId, inNode );
        return theId;
    }

    template< typename NodeType >
    void
    vkrt::gui::Graph< NodeType >::insertNode( const std::size_t id, NodeType & inNode )
    {
        assert( !mNodes.contains( id ) );
        mNodes.insert( mCurrentId, inNode );
    }

    template< typename NodeType >
    void
    vkrt::gui::Graph< NodeType >::removeNode( const std::size_t inId )
    {
        assert( mEdges.contains( inId ) );
        mNodes.remove( inId );
    }

    template< typename NodeType >
    std::size_t
    vkrt::gui::Graph< NodeType >::addEdge( const std::size_t src, const std::size_t dst )
    {
        const std::size_t theId = mCurrentId++;
        Edge theEdge;
        theEdge.mId = theId;
        theEdge.mSrcNodeId = src;
        theEdge.mDstNodeId = dst;
        mEdges.insert( mCurrentId, theEdge );
        return theId;
    }

    template< typename NodeType >
    void
    vkrt::gui::Graph< NodeType >::insertEdge( const std::size_t id, const std::size_t src, const std::size_t dst )
    {
        assert( !mEdges.contains( id ) );
        Edge theEdge = { id, src, dst };
        mEdges.insert( mCurrentId, theEdge );
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
        for( auto theId : graph.GetNodeIds() )
        {
            theIdStack.push_back( theId );
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
