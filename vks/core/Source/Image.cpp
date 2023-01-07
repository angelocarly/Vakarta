//
// Created by Angelo Carly on 30/11/2022.
//

#include "vks/core/Image.h"

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.hpp>

vks::Image::Image()
{}

vks::Image::Image( vk::Image inImage, vma::Allocation inAllocation )
:
    mImage( inImage ),
    mAllocation( inAllocation )
{

}

vks::Image::~Image()
{}

vk::Image
vks::Image::GetVkImage()
{
    return mImage;
}

vma::Allocation
vks::Image::GetAllocation()
{
    return mAllocation;
}
