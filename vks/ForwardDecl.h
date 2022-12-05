//
// Created by Angelo Carly on 26/11/2022.
//

#ifndef VKRT_FORWARDDECL_H
#define VKRT_FORWARDDECL_H

#include <memory>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_beta.h>

namespace vks
{
    const std::vector< const char * > kEnabledExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
//        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
//        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
    };

    class PhysicalDevice;
    typedef std::shared_ptr< vks::PhysicalDevice > PhysicalDevicePtr;
    class LogicalDevice;
    typedef std::shared_ptr< vks::LogicalDevice > LogicalDevicePtr;

}

#endif //VKRT_FORWARDDECL_H
