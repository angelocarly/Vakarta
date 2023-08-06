//
// Created by magnias on 7/29/23.
//

#ifndef Vox_ParticleCompute_h
#define Vox_ParticleCompute_h

#include "vks/core/Buffer.h"
#include "vks/render/ForwardDecl.h"
#include "VoxelControls.h"

#include <vulkan/vulkan.hpp>

#include <chrono>

namespace Vox
{
    /**
     * Cellular automata compute pipeline for voxels
     */
    class ParticleCompute
    {
        public:
            ParticleCompute( vks::DevicePtr inDevice, std::size_t inWorldSize, std::shared_ptr< Vox::VoxelControls > inControls );
            ~ParticleCompute();

            void Compute( vk::CommandBuffer const inCommandBuffer, vks::Buffer & inWorldBuffer );

        private:
            void InitializeDescriptorSetLayout();
            void InitializeComputePipeline();
            void InitializeBuffers();

        private:
            vks::DevicePtr mDevice;
            vk::DescriptorSetLayout mDescriptorSetLayout;
            vks::ComputePipelinePtr mComputePipeline;

            std::shared_ptr< Vox::VoxelControls > mControls;

            std::size_t const kWorldSize;
            vk::RenderPass const kRenderPass;

            std::size_t const kParticleCount = 1000;
            vks::Buffer mParticleBuffer;

            std::chrono::microseconds mStartTime;
            struct PushConstants
            {
                float mTime;
            };
    };
}

#endif
