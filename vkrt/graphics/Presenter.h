/**
 * Presenter.h
 *
 * @file	Presenter.h
 * @author	Angelo Carly
 * @date	11/06/2023
 *
 */

#ifndef VKRT_PRESENTER_H
#define VKRT_PRESENTER_H

#include "RenderEnvironment.h"

#include <cstddef>

namespace vkrt
{
    class Presenter
    {
        public:

            /**
             * Prepare data for rendering
             * @param inFrameIndex
             */
            virtual void Prepare( vkrt::RenderEnvironment const & inEnvironment ) = 0;

            /**
             * Render a mesh to the final framebuffer
             * @param inFrameIndex
             */
            virtual void Draw( vkrt::RenderEnvironment const & inEnvironment ) = 0;
    };
}

#endif
