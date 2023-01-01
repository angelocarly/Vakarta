//
// Created by Angelo Carly on 18/12/2022.
//

#ifndef VKRT_PIPELINE_H
#define VKRT_PIPELINE_H

#include "vks/ForwardDecl.h"

#include <vulkan/vulkan.hpp>

#include <memory>
#include <glm/mat4x4.hpp>

namespace vks
{
    class Pipeline
    {
        public:
            Pipeline( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass );
            ~Pipeline();

        public:
            void UpdatePipelineUniforms( glm::mat4 inCamera );
            void Bind( vk::CommandBuffer inCommandBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_PIPELINE_H
