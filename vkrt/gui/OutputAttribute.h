/**
 * ResourceSource.h
 *
 * @file	ResourceSource.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_OUTPUTATTRIBUTE_H
#define VKRT_OUTPUTATTRIBUTE_H

#include "GuiAttribute.h"
#include "ResourceProvider.h"

namespace vkrt::gui
{
    template< typename T >
    class OutputAttribute
    :
        public GuiAttribute,
        public ResourceProvider< T >
    {
        public:
            OutputAttribute( std::size_t inId, std::function< T( void ) > inCallback );
            ~OutputAttribute();

            virtual T GetResource() override
            {
                return mCallback();
            }

        private:
            std::function< T( void ) > mCallback;

    };

    template< typename T >
    OutputAttribute< T >::OutputAttribute( std::size_t inId, std::function< T( void ) > inCallback )
    :
        GuiAttribute( inId, GuiAttribute::kOutput ),
        mCallback( inCallback )
    {

    }

    template< typename T >
    OutputAttribute< T >::~OutputAttribute()
    {

    }
}

#endif //VKRT_OUTPUTATTRIBUTE_H
