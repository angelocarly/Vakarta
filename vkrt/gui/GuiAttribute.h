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

        public:
            GuiAttribute( std::size_t inId, GuiAttribute::Type inType )
            :
                mId( inId ),
                mType( inType )
            {

            }

            ~GuiAttribute()
            {

            }

        public:
            std::size_t mId;
            GuiAttribute::Type mType;
    };
}

#endif //VKRT_GUIATTRIBUTE_H
