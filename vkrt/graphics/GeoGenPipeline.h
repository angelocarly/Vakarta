//
// Created by Angelo Carly on 18/12/2022.
//

#ifndef VKRT_GEOGENPIPELINE_H
#define VKRT_GEOGENPIPELINE_H

#include "vks/core/Buffer.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/Pipeline.h"

#include <vulkan/vulkan.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

namespace vkrt
{
    class GeoGenPipeline
    {
        public:
            GeoGenPipeline( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass );
            ~GeoGenPipeline();

        public:
            void UpdatePipelineUniforms( glm::mat4 inCamera );
            void Bind( vk::CommandBuffer inCommandBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_GEOGENPIPELINE_H
