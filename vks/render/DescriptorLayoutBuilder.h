//
// Created by Angelo Carly on 14/01/2023.
//

#ifndef VKRT_DESCRIPTORLAYOUTBUILDER_H
#define VKRT_DESCRIPTORLAYOUTBUILDER_H

#include <vulkan/vulkan.hpp>
#include "ForwardDecl.h"

namespace vks
{
    class DescriptorLayoutBuilder
    {
        public:
            DescriptorLayoutBuilder();
            ~DescriptorLayoutBuilder();

        public:
            void AddLayoutBinding( std::uint32_t inBinding, vk::DescriptorType inDescriptorType, vk::ShaderStageFlags inShaderStageFlags );
            std::vector< vk::DescriptorSetLayout > Build( vks::DevicePtr inDevice );

        private:
            std::vector< vk::DescriptorSetLayoutBinding > mDescriptorSetLayoutBindings;
    };
}


#endif //VKRT_DESCRIPTORLAYOUTBUILDER_H
