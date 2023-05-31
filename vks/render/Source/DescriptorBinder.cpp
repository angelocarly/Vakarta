/**
 * DescriptorBinder.cpp
 *
 * @file	DescriptorBinder.cpp
 * @author	Angelo Carly
 * @date	08/05/2023
 *
 */

#include "vks/render//DescriptorBinder.h"

#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"

#include <vulkan/vulkan.hpp>

vks::DescriptorBinder::DescriptorBinder( vks::DevicePtr inDevice, vk::DescriptorSet inDescriptorSet )
:
    mDevice( inDevice ),
    mDescriptorSet( inDescriptorSet )
{

}

vks::DescriptorBinder::~DescriptorBinder()
{

}

vks::DescriptorBinder
vks::DescriptorBinder::AddBufferBinding( std::uint32_t inBinding, vk::DescriptorType inType, vk::DescriptorBufferInfo const & inPBufferInfo )
{
    auto theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstSet( mDescriptorSet );
    theWriteDescriptorSet.setDstBinding( inBinding );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( inType );
    theWriteDescriptorSet.setDescriptorCount( 1 );
    theWriteDescriptorSet.setPBufferInfo( &inPBufferInfo );

    mWriteDescriptorSets.push_back( theWriteDescriptorSet );

    return *this;
}

vks::DescriptorBinder
vks::DescriptorBinder::AddBufferArrayBinding( std::uint32_t inBinding, vk::DescriptorType inType, std::vector< vk::DescriptorBufferInfo > & inBufferInfo )
{
    auto theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstSet( mDescriptorSet );
    theWriteDescriptorSet.setDstBinding( inBinding );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( inType );
    theWriteDescriptorSet.setDescriptorCount( inBufferInfo.size() );
    theWriteDescriptorSet.setPBufferInfo( inBufferInfo.data() );
    theWriteDescriptorSet.setPImageInfo( {} );

    mWriteDescriptorSets.push_back( theWriteDescriptorSet );

    return *this;
}

vks::DescriptorBinder
vks::DescriptorBinder::AddImageBinding( std::uint32_t inBinding, vk::DescriptorType inType, vk::DescriptorImageInfo const & inPImageInfo )
{
    auto theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstSet( mDescriptorSet );
    theWriteDescriptorSet.setDstBinding( inBinding );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( inType );
    theWriteDescriptorSet.setDescriptorCount( 1 );
    theWriteDescriptorSet.setPImageInfo( &inPImageInfo );

    mWriteDescriptorSets.push_back( theWriteDescriptorSet );

    return *this;
}

vks::DescriptorBinder
vks::DescriptorBinder::AddImageArrayBinding( std::uint32_t inBinding, vk::DescriptorType inType, std::vector< vk::DescriptorImageInfo > & inImageInfos )
{
    auto theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstSet( mDescriptorSet );
    theWriteDescriptorSet.setDstBinding( inBinding );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( inType );
    theWriteDescriptorSet.setDescriptorCount( inImageInfos.size() );
    theWriteDescriptorSet.setPImageInfo( inImageInfos.data() );

    mWriteDescriptorSets.push_back( theWriteDescriptorSet );

    return *this;
}

void
vks::DescriptorBinder::Bind()
{
    mDevice->GetVkDevice().updateDescriptorSets( mWriteDescriptorSets, {} );
}

