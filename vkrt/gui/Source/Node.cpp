//
// Created by magnias on 4/23/23.
//

#include "vkrt/gui/Node.h"

vkrt::gui::Node::Node( vkrt::gui::NodeContextPtr inContext, Type mType )
:
    mId( inContext->AddNode( this ) ),
    mType( mType )
{
}

vkrt::gui::Node::~Node()
{

}
