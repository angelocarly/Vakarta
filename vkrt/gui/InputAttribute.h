/**
 * ResourceSink.h
 *
 * @file	ResourceSink.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_INPUTATTRIBUTE_H
#define VKRT_INPUTATTRIBUTE_H

#include "GuiAttribute.h"
#include "ResourceProvider.h"

namespace vkrt::gui
{
    template< typename T >
    class InputAttribute
    :
        public GuiAttribute
    {
        public:
            InputAttribute( std::size_t inId );
            ~InputAttribute();

            void Connect( std::shared_ptr< vkrt::gui::ResourceProvider< T > > inProvider );
            void ResetConnection();
            bool IsConnected() const;
            T GetResource() const;

        private:
            std::shared_ptr< vkrt::gui::ResourceProvider< T > > mProvider;
    };

    template< typename T >
    T InputAttribute< T >::GetResource() const
    {
        if( IsConnected() ) return mProvider->GetResource();
        return nullptr;
    }

    template< typename T >
    void InputAttribute< T >::ResetConnection()
    {
        mProvider = nullptr;
    }

    template< typename T >
    bool InputAttribute< T >::IsConnected() const
    {
        return mProvider != nullptr;
    }

    template< typename T >
    InputAttribute< T >::InputAttribute( std::size_t inId )
    :
        GuiAttribute( inId, GuiAttribute::kInput )
    {

    }

    template< typename T >
    InputAttribute< T >::~InputAttribute()
    {

    }

    template< typename T >
    void
    vkrt::gui::InputAttribute< T >::Connect( std::shared_ptr< vkrt::gui::ResourceProvider< T > > inProvider )
    {
        mProvider = std::move( inProvider );
    }
}

#endif //VKRT_INPUTATTRIBUTE_H
