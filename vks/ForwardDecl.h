//
// Created by Angelo Carly on 26/11/2022.
//

#ifndef VKS_FORWARDDECL_H
#define VKS_FORWARDDECL_H

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

    class Swapchain;
    typedef std::shared_ptr< vks::Swapchain > SwapchainPtr;
    class RenderPass;
    typedef std::shared_ptr< vks::RenderPass > RenderPassPtr;
    class PhysicalDevice;
    typedef std::shared_ptr< vks::PhysicalDevice > PhysicalDevicePtr;
    class Device;
    typedef std::shared_ptr< vks::Device > DevicePtr;

}

#endif //VKRT_FORWARDDECL_H
