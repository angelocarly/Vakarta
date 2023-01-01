//
// Created by Angelo Carly on 18/12/2022.
//

#ifndef VKRT_PIPELINE_H
#define VKRT_PIPELINE_H

#include "vks/ForwardDecl.h"

#include <vulkan/vulkan.hpp>

#include <memory>

namespace vks
{
    class Pipeline
    {
        public:
            Pipeline( vks::DevicePtr inDevice, vks::RenderPassPtr inRenderPass );
            ~Pipeline();

        public:
            void UpdatePipelineUniforms( int inWidth, int inHeight );
            void Bind( vk::CommandBuffer inCommandBuffer );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_PIPELINE_H
