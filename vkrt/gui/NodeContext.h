//
// Created by magnias on 4/23/23.
//

#ifndef VKRT_NODECONTEXT_H
#define VKRT_NODECONTEXT_H

#include <utility>
#include <vector>
#include <map>
#include "IdCache.h"
#include "ForwardDecl.h"
#include "GuiAttribute.h"
#include "InputAttribute.h"
#include "OutputAttribute.h"
#include "Link.h"

#include <vulkan/vulkan.hpp>

namespace vkrt::gui
{

    class NodeContext
    {
        public:
            NodeContext();
            ~NodeContext();

            std::size_t GetNextLinkId() { return mLinks.size(); }
            std::size_t AddNode( Node * inNode );
            NodePtr GetNode( std::size_t inIndex ) { return mNodes[ inIndex ]; };
            std::size_t AddLink( std::shared_ptr< vkrt::gui::OutputAttribute< vk::DescriptorSet > > a, std::shared_ptr< vkrt::gui::InputAttribute< vk::DescriptorSet > > b );
            [[nodiscard]] std::map< std::size_t, std::shared_ptr< vkrt::gui::Link< vk::DescriptorSet > > > GetLinks() const { return mLinks; }
            [[nodiscard]] std::map< std::size_t, NodePtr > GetNodes() const { return mNodes; }
            void RemoveLink( std::size_t inId );

        private:
            std::size_t AddAttribute();

        private:
            std::map< std::size_t, std::shared_ptr< vkrt::gui::Link< vk::DescriptorSet > > > mLinks;
            std::map< std::size_t, NodePtr > mNodes;
            std::map< std::size_t, std::shared_ptr< vkrt::gui::GuiAttribute > > mAttributes;
            vkrt::gui::IdCache mLinkIdCache;
            vkrt::gui::IdCache mNodeIdCache;
            vkrt::gui::IdCache mAttributeIdCache;

        public:

            std::shared_ptr< vkrt::gui::InputAttribute< vk::DescriptorSet > >
            AddInputAttribute()
            {
                auto theId = mAttributeIdCache.AddId();
                auto theAttribute = std::make_shared< vkrt::gui::InputAttribute< vk::DescriptorSet > >( theId );
                mAttributes[ theId ] = theAttribute;
                return theAttribute;
            }

            std::shared_ptr< vkrt::gui::OutputAttribute< vk::DescriptorSet > >
            AddOutputAttribute( std::function< vk::DescriptorSet( void ) > inCallback )
            {
                auto theId = mAttributeIdCache.AddId();
                auto t = vkrt::gui::OutputAttribute< vk::DescriptorSet >( 0, inCallback );
                auto theAttribute = std::make_shared< vkrt::gui::OutputAttribute< vk::DescriptorSet > >( theId, inCallback );
                mAttributes[ theId ] = theAttribute;
                return theAttribute;
            }

            std::shared_ptr< vkrt::gui::InputAttribute< vk::DescriptorSet > >
            GetInputAttribute( std::size_t inAttributeId )
            {
                auto theAttribute = mAttributes[ inAttributeId ];
                if( !theAttribute )
                {
                    throw std::runtime_error( "Attribute not found" );
                }

                if( theAttribute->mType != GuiAttribute::Type::kInput )
                {
                    throw std::runtime_error( "Attribute is not an input attribute" );
                }

                return std::static_pointer_cast< vkrt::gui::InputAttribute< vk::DescriptorSet > >( theAttribute );
            }

            std::shared_ptr< vkrt::gui::OutputAttribute< vk::DescriptorSet > >
            GetOutputAttribute( std::size_t inAttributeId )
            {
                auto theAttribute = mAttributes[ inAttributeId ];
                if( !theAttribute )
                {
                    throw std::runtime_error( "Attribute not found" );
                }

                if( theAttribute->mType != GuiAttribute::Type::kOutput )
                {
                    throw std::runtime_error( "Attribute is not an output attribute" );
                }

                return std::static_pointer_cast< vkrt::gui::OutputAttribute< vk::DescriptorSet > >( theAttribute );
            }
    };

}

#endif //VKRT_NODECONTEXT_H
