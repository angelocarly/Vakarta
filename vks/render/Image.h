//
// Created by Angelo Carly on 30/11/2022.
//

#ifndef PACKZ_IMAGE_H
#define PACKZ_IMAGE_H

#include "ForwardDecl.h"

#include "vulkan/vulkan.hpp"
#include "vk_mem_alloc.hpp"

namespace vks
{
    class Image
    {
        public:
            Image();
            Image( vk::Image inImage, vma::Allocation inAllocation );
            ~Image();

        public:
            vk::Image GetVkImage();
            vma::Allocation GetAllocation();

        private:
            vk::Image mImage;
            vma::Allocation mAllocation;
    };
}

#endif //PACKZ_IMAGE_H
