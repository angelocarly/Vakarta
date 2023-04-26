//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/NodeContext.h"
#include "vkrt/gui/Node.h"

vkrt::gui::NodeContext::NodeContext()
{

}

vkrt::gui::NodeContext::~NodeContext()
{

}

std::size_t
vkrt::gui::NodeContext::AddNode( Node * inNode )
{
    auto theId = mNodeIdCache.AddId();
    mNodes[ theId ] = std::shared_ptr< Node >( inNode );
    return theId;
}

std::size_t
vkrt::gui::NodeContext::AddAttribute()
{
    return mAttributes.AddId();
}


std::size_t
vkrt::gui::NodeContext::AddLink( std::size_t a, std::size_t b )
{
    std::size_t theId = mLinkIdCache.AddId();
    mLinks[ theId ] = { a, b };
    return theId;
}

void
vkrt::gui::NodeContext::RemoveLink( std::size_t inId )
{
    mLinkIdCache.RemoveId( inId );
    mLinks.erase( inId );
}

