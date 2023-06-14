/**
 * ResourceSource.h
 *
 * @file	ResourceSource.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 *
 */

#ifndef VKRT_OUTPUTATTRIBUTE_H
#define VKRT_OUTPUTATTRIBUTE_H

#include "GuiAttribute.h"
#include "ResourceProvider.h"
#include "UpdateListener.h"
#include "NodeContext.h"
#include "ForwardDecl.h"

#include <functional>

namespace vkrt::gui
{
    template< typename T >
    class OutputAttribute
    :
        public GuiAttribute,
        public ResourceProvider< T >
    {
        public:
            OutputAttribute( std::function< std::optional< T >( void ) > inCallback );
            ~OutputAttribute();

            void Connect( std::shared_ptr< vkrt::gui::UpdateListener > inLink );
            void Disconnect( std::shared_ptr< vkrt::gui::UpdateListener > inLink );
            void Update() override;

            virtual std::optional< T > GetResource() override
            {
                return mCallback();
            }

        private:
            std::function< std::optional< T >( void ) > mCallback;
            std::vector< std::shared_ptr< vkrt::gui::UpdateListener > > mLinks;

    };

    template< typename T >
    void
    OutputAttribute< T >::Connect( std::shared_ptr< vkrt::gui::UpdateListener > inLink )
    {
        // Check if links already contained
        for( auto theLink : mLinks )
        {
            if( theLink == inLink )
            {
                return;
            }
        }

        // Add the link
        mLinks.push_back( inLink );
    }

    template< typename T >
    void
    OutputAttribute< T >::Disconnect( std::shared_ptr< vkrt::gui::UpdateListener > inLink )
    {
        // Delete the link in one line
        mLinks.erase( std::remove( mLinks.begin(), mLinks.end(), inLink ), mLinks.end() );

    }

    template< typename T >
    void
    OutputAttribute< T >::Update()
    {
        for( auto theLink : mLinks )
        {
            theLink->Update();
        }
    }

    template< typename T >
    OutputAttribute< T >::OutputAttribute( std::function< std::optional< T >( void ) > inCallback )
    :
        GuiAttribute( -1, GuiAttribute::kOutput, ResourceType::kImage ),
        mCallback( inCallback )
    {

    }

    template< typename T >
    OutputAttribute< T >::~OutputAttribute()
    {

    }
}

#endif //VKRT_OUTPUTATTRIBUTE_H
