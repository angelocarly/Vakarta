#ifndef VKRT_GRAPHICS_LAYER_H
#define VKRT_GRAPHICS_LAYER_H

#include <vulkan/vulkan.hpp>
#include "vkrt/graphics/RenderEnvironment.h"

namespace vkrt
{
    /**
     * A layer in the displayer
     */
    class Layer
    {
        public:
            /**
             * @brief Update the layer's image
             */
            virtual void Render( vkrt::RenderEnvironment & inRenderEnvironment ) = 0;
    };
}

#endif
