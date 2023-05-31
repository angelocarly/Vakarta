/**
 * DescriptorBinder.h
 *
 * @file	DescriptorBinder.h
 * @author	Angelo Carly
 * @date	08/05/2023
 *
 */

#ifndef VKRT_DESCRIPTORBINDER_H
#define VKRT_DESCRIPTORBINDER_H

#include "vks/render/ForwardDecl.h"

#include <vulkan/vulkan.hpp>
#include <cstdint>

namespace vks
{
    class DescriptorBinder
    {
        public:
        DescriptorBinder( vks::DevicePtr inDevice, vk::DescriptorSet inDescriptorSet );
        ~DescriptorBinder();

        public:
        DescriptorBinder AddBufferBinding( std::uint32_t inBinding, vk::DescriptorType inType, vk::DescriptorBufferInfo const & inPBufferInfo );
        DescriptorBinder AddBufferArrayBinding( std::uint32_t inBinding, vk::DescriptorType inType, std::vector< vk::DescriptorBufferInfo > & inBufferInfo );
        DescriptorBinder AddImageBinding( std::uint32_t inBinding, vk::DescriptorType inType, vk::DescriptorImageInfo const & inPImageInfo );
        DescriptorBinder AddImageArrayBinding( std::uint32_t inBinding, vk::DescriptorType inType, std::vector< vk::DescriptorImageInfo > & inImageInfo );
        void Bind();

        private:
            vks::DevicePtr mDevice;
            vk::DescriptorSet mDescriptorSet;

            std::vector< vk::WriteDescriptorSet > mWriteDescriptorSets;
    };
}


#endif //VKRT_DESCRIPTORBINDER_H
