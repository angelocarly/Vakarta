//
// Created by Angelo Carly on 27/12/2022.
//

#ifndef VKRT_BUFFER_H
#define VKRT_BUFFER_H

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.hpp>

namespace vks
{
    class Buffer
    {
        public:
            Buffer();
            Buffer( vk::Buffer inBuffer, vma::Allocation inAllocation );

        public:
            vk::Buffer GetVkBuffer();
            vma::Allocation GetVmaAllocation();

        private:
            vk::Buffer mBuffer;
            vma::Allocation mAllocation;
    };
}


#endif //VKRT_BUFFER_H
