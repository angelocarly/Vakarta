//
// Created by Angelo Carly on 11/01/2023.
//

#ifndef VKRT_LINEPIPELINE_H
#define VKRT_LINEPIPELINE_H

#include "vks/render/Pipeline.h"

#include <glm/glm.hpp>

namespace vkrt
{
    class LinePipeline
    {
        public:
            LinePipeline( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass );
            ~LinePipeline();

        public:
            void UpdatePipelineUniforms( glm::mat4 inCamera );
            void Bind( vk::CommandBuffer inCommandBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_LINEPIPELINE_H
