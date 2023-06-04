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
#include "vkrt/gui/OutputAttribute.h"
#include "OutputAttribute.h"

namespace vkrt::gui
{
    class LinkHandle
    {
        public:
            LinkHandle( std::size_t inLinkId, std::size_t inStartAttributeId, std::size_t inEndAttributeId )
            :
                mLinkId( inLinkId ),
                mStartAttributeId( inStartAttributeId ),
                mEndAttributeId( inEndAttributeId )
            {}
            ~LinkHandle()
            {}

            virtual void Disconnect() = 0;

        public:
            std::size_t mLinkId;
            std::size_t mStartAttributeId;
            std::size_t mEndAttributeId;
    };

    template< typename T >
    class Link
    :
        public ResourceProvider< T >,
        public LinkHandle,
        public UpdateListener
    {
        public:
            Link( std::size_t inLinkId, std::shared_ptr< vkrt::gui::OutputAttribute< T > > inSource, std::shared_ptr< vkrt::gui::InputAttribute< T > > inDestination )
            :
                LinkHandle( inLinkId, inSource->mId, inDestination->mId ),
                mSource( std::move( inSource ) ),
                mDestination( std::move( inDestination ) )
            {

            }

            ~Link()
            {

            }

            void Update()
            {
                mDestination->Update();
            }

            std::optional< T > GetResource() override
            {
                return mSource->GetResource();
            }

            void Draw()
            {

            }

            virtual void Disconnect() override
            {
                mDestination->Disconnect();
            }

        public:
            std::shared_ptr< vkrt::gui::OutputAttribute< T > > mSource;
            std::shared_ptr< vkrt::gui::InputAttribute< T > > mDestination;
    };
}

#endif //VKRT_LINK_H
