//
// Created by Angelo Carly on 14/01/2023.
//

#ifndef VKRT_DESCRIPTORSET_H
#define VKRT_DESCRIPTORSET_H

#include "vks/core/Buffer.h"
#include "vks/render/ForwardDecl.h"

namespace vks
{
    class DescriptorSet
    {
        public:
            DescriptorSet( vks::DevicePtr inDevice, vk::DescriptorPool inDescriptorPool, vk::DescriptorSetLayout inLayout );
            ~DescriptorSet();

        public:
            vks::DescriptorSet
            BindBuffer( std::uint32_t inBinding, vk::DescriptorType inDescriptorType, vks::Buffer inBuffer );
            vk::DescriptorSet GetVkDescriptorSet();

        private:
            vks::DevicePtr mDevice;
            vk::DescriptorSet mDescriptorSet;
    };
}


#endif //VKRT_DESCRIPTORSET_H
