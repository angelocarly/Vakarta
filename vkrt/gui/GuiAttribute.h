/**
 * GuiAttribute.h
 *
 * @file	GuiAttribute.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_GUIATTRIBUTE_H
#define VKRT_GUIATTRIBUTE_H

#include <cstdio>
#include "Graph.h"

namespace vkrt::gui
{
    class GuiAttribute
    {
        public:
            enum Type
            {
                kInput,
                kOutput
            };
            enum ResourceType
            {
                kImage
            };

        public:
            GuiAttribute( std::size_t inId, GuiAttribute::Type inType, ResourceType inResourceType )
            :
                mId( inId ),
                mNodeId( -1 ),
                mType( inType ),
                mResourceType( inResourceType )
            {
            }

            ~GuiAttribute()
            {

            }

        public:
            std::size_t mId;
            std::size_t mNodeId;
            GuiAttribute::Type mType;
            GuiAttribute::ResourceType mResourceType;

            void SetId( std::size_t inId ) { mId = inId; }
    };
}

#endif //VKRT_GUIATTRIBUTE_H
