//
// Created by Angelo Carly on 14/01/2023.
//

#include "vks/render/DescriptorSet.h"

#include "vks/core/Buffer.h"
#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"

#include <vulkan/vulkan.hpp>

vks::DescriptorSet::DescriptorSet( vks::DevicePtr inDevice, vk::DescriptorPool inDescriptorPool, vk::DescriptorSetLayout inLayout )
:
    mDevice( inDevice )
{
    // Allocate a descriptor set from the pool
    vk::DescriptorSetAllocateInfo const theDescriptorSetAllocateInfo
    (
        inDescriptorPool,
        std::uint32_t( 1 ),
        &inLayout
    );

    mDescriptorSet = inDevice->GetVkDevice().allocateDescriptorSets( theDescriptorSetAllocateInfo ).front();
}

vks::DescriptorSet::~DescriptorSet()
{

}

vks::DescriptorSet
vks::DescriptorSet::BindBuffer( std::uint32_t inBinding, vk::DescriptorType inDescriptorType, vks::Buffer inBuffer )
{
    // Configure the descriptors
    std::array< vk::WriteDescriptorSet, 1 > theWriteDescriptorSet;

    // Uniform buffer
    vk::DescriptorBufferInfo const theDescriptorBufferInfo
    (
        inBuffer.GetVkBuffer(),
        0,
        inBuffer.GetSize()
    );
    theWriteDescriptorSet[ 0 ].setDstSet( mDescriptorSet );
    theWriteDescriptorSet[ 0 ].setDstBinding( inBinding );
    theWriteDescriptorSet[ 0 ].setDstArrayElement( 0 );
    theWriteDescriptorSet[ 0 ].setDescriptorType( inDescriptorType );
    theWriteDescriptorSet[ 0 ].setDescriptorCount( 1 );
    theWriteDescriptorSet[ 0 ].setPBufferInfo( &theDescriptorBufferInfo );

    mDevice->GetVkDevice().updateDescriptorSets( theWriteDescriptorSet, {} );

    return *this;
}

vk::DescriptorSet vks::DescriptorSet::GetVkDescriptorSet()
{
    return mDescriptorSet;
}
