//
// Created by Angelo Carly on 30/11/2022.
//

#include "vks/core/Image.h"

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.hpp>

vks::Image::Image()
{}

vks::Image::Image( vk::Image inImage, vma::Allocation inAllocation, std::size_t inWidth, std::size_t inHeight )
:
    mImage( inImage ),
    mAllocation( inAllocation ),
    mWidth( inWidth ),
    mHeight( inHeight )
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
