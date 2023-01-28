/*
 * A Descriptor Layout contains how a descriptor is structured.
 * It contains layout bindings describing what 'buffer bindings' go where.
 */

#include "vks/render/DescriptorLayoutBuilder.h"

#include "vks/render/Device.h"

#include <vulkan/vulkan.hpp>

vks::DescriptorLayoutBuilder::DescriptorLayoutBuilder() = default;

vks::DescriptorLayoutBuilder::~DescriptorLayoutBuilder() = default;

vks::DescriptorLayoutBuilder
vks::DescriptorLayoutBuilder::AddLayoutBinding( uint32_t inBinding, vk::DescriptorType inDescriptorType, vk::ShaderStageFlags inShaderStageFlags )
{
    auto theDescriptorSetLayoutBinding = vk::DescriptorSetLayoutBinding
    (
        inBinding,
        inDescriptorType,
        1,
        inShaderStageFlags,
        nullptr
    );
    mDescriptorSetLayoutBindings.push_back( theDescriptorSetLayoutBinding );

    return *this;
}

std::vector< vk::DescriptorSetLayout >
vks::DescriptorLayoutBuilder::Build( vks::DevicePtr inDevice )
{
    auto theDescriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo
    (
        vk::DescriptorSetLayoutCreateFlags(),
        mDescriptorSetLayoutBindings.size(),
        mDescriptorSetLayoutBindings.data()
    );

    std::vector< vk::DescriptorSetLayout > theDescriptorSetLayouts;
    theDescriptorSetLayouts.push_back
    (
        inDevice->GetVkDevice().createDescriptorSetLayout
        (
            theDescriptorSetLayoutCreateInfo
        )
    );

    return theDescriptorSetLayouts;
}
