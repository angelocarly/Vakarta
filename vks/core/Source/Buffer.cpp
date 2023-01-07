//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/core/Buffer.h"

vks::Buffer::Buffer()
{

}

vks::Buffer::Buffer( vk::Buffer inBuffer, vma::Allocation inAllocation )
:
    mBuffer( inBuffer ),
    mAllocation( inAllocation )
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
