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
    return mAttributeIdCache.AddId();
}


std::size_t
vkrt::gui::NodeContext::AddLink
(
    std::shared_ptr< vkrt::gui::OutputAttribute< vk::DescriptorSet > > a,
    std::shared_ptr< vkrt::gui::InputAttribute< vk::DescriptorSet > > b
)
{
    std::size_t theId = mLinkIdCache.AddId();
    auto theLink = std::make_shared< vkrt::gui::Link< vk::DescriptorSet > >( theId, a, b );
    mLinks[ theId ] = theLink;

    b->Connect( theLink );

    return theId;
}

void
vkrt::gui::NodeContext::RemoveLink( std::size_t inId )
{
    auto theLink = mLinks[ inId ];

    theLink->mDestination->ResetConnection();

    mLinkIdCache.RemoveId( inId );
    mLinks.erase( inId );
}

