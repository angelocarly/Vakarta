/**
 * ResourceSource.h
 *
 * @file	ResourceSource.h
 * @author	Angelo Carly
 * @date	28/04/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef VKRT_RESOURCEPROVIDER_H
#define VKRT_RESOURCEPROVIDER_H

namespace vkrt::gui
{
    template< typename T >
    class ResourceProvider
    {
        public:
            virtual T GetResource() = 0;

    };

}

#endif //VKRT_RESOURCEPROVIDER_H
