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
    static std::vector< const char * > GetRequiredExtensions()
    {
        static std::vector< const char * > theExtensions;
        theExtensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
#if TARGET_OS_X
        theExtensions.push_back( VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME );
#endif
        return theExtensions;
    }

    class Swapchain;
    typedef std::shared_ptr< vks::Swapchain > SwapchainPtr;
    class RenderPass;
    typedef std::shared_ptr< vks::RenderPass > RenderPassPtr;
    class PhysicalDevice;
    typedef std::shared_ptr< vks::PhysicalDevice > PhysicalDevicePtr;
    class Device;
    typedef std::shared_ptr< vks::Device > DevicePtr;
    class Window;
    typedef std::shared_ptr< vks::Window > WindowPtr;
    class GuiPass;
    typedef std::shared_ptr< vks::GuiPass > ImGuiPtr;

}

#endif //VKRT_FORWARDDECL_H
