//
// Created by magnias on 7/7/22.
//

#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include "vks/VksInstance.h"

vks::Instance::Instance()
{
    CreateInstance();
}

vks::Instance::~Instance()
{
    vkDestroyInstance(mInstance, nullptr);
}

void vks::Instance::CreateInstance()
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
    createInfo.ppEnabledLayerNames = {};
    createInfo.enabledExtensionCount = 0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> requiredExtensions;
    for(uint32_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.emplace_back(glfwExtensions[i]);
    }

    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    if( vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS )
    {
        spdlog::error("Failed to create a Vulkan instance");
        throw std::runtime_error("Failed to create a Vulkan instance");
    }
}