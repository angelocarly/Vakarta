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
            void Bind( vk::CommandBuffer inCommandBuffer );
            void Dispatch( vk::CommandBuffer inCommandBuffer, vks::Buffer inOutputBuffer, std::uint32_t inVertexCount );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_GEOGENPIPELINE_H
