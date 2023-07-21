/**
 * VoxelCompute.h
 *
 * @file	VoxelCompute.h
 * @author	Angelo Carly
 * @date	27/06/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#ifndef Mttr_VoxelCompute_h
#define Mttr_VoxelCompute_h

#include "vks/core/Buffer.h"
#include "vks/render/ForwardDecl.h"
#include "VoxelControls.h"

#include <vulkan/vulkan.hpp>

#include <chrono>

namespace Mttr::Vox
{
    /**
     * Cellular automata compute pipeline for voxels
     */
    class VoxelCompute
    {
        public:
            VoxelCompute( vks::DevicePtr inDevice, std::size_t inWorldSize, std::shared_ptr< Mttr::Vox::VoxelControls > inControls );
            ~VoxelCompute();

            void Compute( vk::CommandBuffer const inCommandBuffer, vks::Buffer & inWorldBuffer );

        private:
            void InitializeDescriptorSetLayout();
            void InitializeComputePipeline();
            void InitializeBuffers();

        private:
            vks::DevicePtr mDevice;
            vk::DescriptorSetLayout mDescriptorSetLayout;
            vks::ComputePipelinePtr mComputePipeline;

            std::shared_ptr< Mttr::Vox::VoxelControls > mControls;

            std::size_t const kWorldSize;
            vk::RenderPass const kRenderPass;

            vks::Buffer mReadWorldBuffer;

            std::chrono::microseconds mStartTime;
            struct PushConstants
            {
                float mTime;
            };
    };
}

#endif
