//
// Created by Angelo Carly on 30/11/2022.
//

#ifndef PACKZ_IMAGE_H
#define PACKZ_IMAGE_H

#include "vks/render/ForwardDecl.h"

#include "vulkan/vulkan.hpp"
#include "vk_mem_alloc.hpp"

namespace vks
{
    class Image
    {
        public:
            Image();
            Image( vk::Image inImage, vma::Allocation inAllocation, std::size_t inWidth, std::size_t inHeight );
            ~Image();

        public:
            vk::Image GetVkImage();
            vma::Allocation GetAllocation();
            std::size_t GetWidth() const { return mWidth; };
            std::size_t GetHeight() const { return mHeight; };

        private:
            vk::Image mImage;
            vma::Allocation mAllocation;
            std::size_t mWidth;
            std::size_t mHeight;
    };
}

#endif //PACKZ_IMAGE_H
