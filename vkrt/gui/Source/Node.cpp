//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/Node.h"

vkrt::gui::Node::Node( vkrt::gui::NodeContextPtr inContext )
    :
    mId( inContext->AddNode() )
{
}

vkrt::gui::Node::~Node()
{

}
