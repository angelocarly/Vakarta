//
// Created by magnias on 7/7/22.
//

#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include "vks_device.h"

VksDevice::VksDevice()
{
    createInstance();
}

VksDevice::~VksDevice()
{

}

void VksDevice::createInstance()
{
    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "VKRT";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "VKRT";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledLayerCount = 0;
//    createInfo.ppEnabledLayerNames = []; // No enabled layers
    createInfo.enabledExtensionCount = 0;
//    createInfo.ppEnabledExtensionNames = []; // No enabled extensions
    VkInstance instance {};

    if( vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS )
    {
        spdlog::error("err");
    }
}
