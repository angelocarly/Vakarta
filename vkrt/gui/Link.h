/**
 * Link.h
 *
 * @file	Link.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_LINK_H
#define VKRT_LINK_H

#include "ForwardDecl.h"
#include "ResourceProvider.h"
#include "InputAttribute.h"
#include "OutputAttribute.h"

namespace vkrt::gui
{
    template< typename T >
    class Link
    :
        public ResourceProvider< T >
    {
        public:
            Link( std::size_t inLinkId, std::shared_ptr< vkrt::gui::OutputAttribute< T > > inSource, std::shared_ptr< vkrt::gui::InputAttribute< T > > inDestination )
            :
                mLinkId( inLinkId ),
                mSource( std::move( inSource ) ),
                mDestination( std::move( inDestination ) )
            {

            }

            ~Link()
            {

            }

            T GetResource() override
            {
                return mSource->GetResource();
            }

            void Draw()
            {

            }

        public:
            std::size_t mLinkId;
            std::shared_ptr< vkrt::gui::OutputAttribute< T > > mSource;
            std::shared_ptr< vkrt::gui::InputAttribute< T > > mDestination;
    };
}

#endif //VKRT_LINK_H
