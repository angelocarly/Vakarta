//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/NodeContext.h"

#include "vkrt/gui/OutputAttribute.h"
#include "vkrt/gui/Node.h"
#include "vks/core/Image.h"

vkrt::gui::NodeContext::NodeContext()
{

}

vkrt::gui::NodeContext::~NodeContext()
{

}

void
vkrt::gui::NodeContext::AddNode( std::shared_ptr< Node > inNode )
{
    auto theId = mGraph.addNode( inNode );
    inNode->SetId( theId );
}

void
vkrt::gui::NodeContext::AddAttribute( std::shared_ptr< GuiAttribute > inAttribute )
{
    assert( inAttribute != nullptr );
    assert( inAttribute->mId == -1 );
    auto theId = mAttributes.insert( inAttribute );
    inAttribute->SetId( theId );
}

void
vkrt::gui::NodeContext::AddLink( const std::size_t inSrc, const std::size_t inDst )
{
    assert( mAttributes.contains( inSrc ) );
    assert( mAttributes.contains( inDst ) );

    std::shared_ptr< GuiAttribute > theSrcAttribute = mAttributes[ inSrc ];
    std::shared_ptr< GuiAttribute > theDstAttribute = mAttributes[ inDst ];
    assert( theSrcAttribute->mType == GuiAttribute::kOutput );
    assert( theDstAttribute->mType == GuiAttribute::kInput );
    assert( theSrcAttribute->mResourceType == theDstAttribute->mResourceType );
    assert( theSrcAttribute->mNodeId != -1 );
    assert( theDstAttribute->mNodeId != -1 );

    // Link the attributes
    switch( theSrcAttribute->mResourceType )
    {
        case GuiAttribute::kImage:
            std::shared_ptr< OutputAttribute< vks::Image > > theSrcImageAttribute = std::static_pointer_cast< OutputAttribute< vks::Image > >( theSrcAttribute );
            std::shared_ptr< InputAttribute< vks::Image > > theDstImageAttribute = std::static_pointer_cast< InputAttribute< vks::Image > >( theDstAttribute );
            theDstImageAttribute->Connect( theSrcImageAttribute );
            theSrcImageAttribute->Connect( theDstImageAttribute );
            break;
    }

    // Store the link
    auto theId = mGraph.addEdge( theSrcAttribute->mNodeId, theDstAttribute->mNodeId );
    mLinks.insert( theId, { theId, inSrc, inDst } );
}

std::vector< vkrt::gui::NodeContext::Link >
vkrt::gui::NodeContext::GetLinks()
{
    return mLinks.elements();
}

void
vkrt::gui::NodeContext::RemoveLink( const std::size_t inId )
{
    // Remove link callback connections
    assert( mLinks.contains( inId ) );

    auto theLink = mLinks.get( inId );
    std::shared_ptr< GuiAttribute > theSrcAttribute = mAttributes[ theLink.mSrcAttribute ];
    std::shared_ptr< GuiAttribute > theDstAttribute = mAttributes[ theLink.mDstAttribute ];
    assert( theSrcAttribute->mType == GuiAttribute::kOutput );
    assert( theDstAttribute->mType == GuiAttribute::kInput );
    assert( theSrcAttribute->mResourceType == theDstAttribute->mResourceType );

    switch( theSrcAttribute->mResourceType )
    {
        case GuiAttribute::kImage:
            std::shared_ptr< OutputAttribute< vks::Image > > theSrcImageAttribute = std::static_pointer_cast< OutputAttribute< vks::Image > >( theSrcAttribute );
            std::shared_ptr< InputAttribute< vks::Image > > theDstImageAttribute = std::static_pointer_cast< InputAttribute< vks::Image > >( theDstAttribute );
            theDstImageAttribute->Disconnect();
            theSrcImageAttribute->Disconnect( theDstImageAttribute );
            break;
    }

    // Remove link data
    mLinks.remove( inId );
    mGraph.removeEdge( inId );
}

void
vkrt::gui::NodeContext::RemoveNode( const std::size_t inId )
{
    mGraph.GetNode( inId );

    // Remove all links to this node
    auto theLinks = mGraph.GetEdges();
    for( auto theLink : theLinks )
    {
        if( theLink.mSrcNodeId == inId || theLink.mDstNodeId == inId )
        {
            RemoveLink( theLink.mId );
        }
    }

    // Remove the node
    mGraph.removeNode( inId );
}

std::vector< std::shared_ptr< vkrt::gui::Node > >
vkrt::gui::NodeContext::GetNodes()
{
    return mGraph.GetNodes();
}

