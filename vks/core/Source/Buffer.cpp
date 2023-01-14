//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/core/Buffer.h"

vks::Buffer::Buffer()
{

}

vks::Buffer::Buffer( vk::Buffer inBuffer, vma::Allocation inAllocation, vk::DeviceSize inSize )
:
    mBuffer( inBuffer ),
    mAllocation( inAllocation ),
    mSize( inSize )
{
}

vk::Buffer
vks::Buffer::GetVkBuffer()
{
    return mBuffer;
}

vma::Allocation
vks::Buffer::GetVmaAllocation()
{
    return mAllocation;
}

vk::DeviceSize
vks::Buffer::GetSize()
{
    return mSize;
}