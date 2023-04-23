//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_GUI_FORWARDDECL_H
#define VKRT_GUI_FORWARDDECL_H

#include <memory>

namespace vkrt::gui
{
    class Node;
    typedef std::shared_ptr< vkrt::gui::Node > NodePtr;

    class NodeContext;
    typedef std::shared_ptr< vkrt::gui::NodeContext > NodeContextPtr;
}

#endif //VKRT_FORWARDDECL_H
