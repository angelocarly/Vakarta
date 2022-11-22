//
// Created by magnias on 7/7/22.
//

#include <vulkan/vulkan.hpp>
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
    vk::ApplicationInfo applicationInfo = vk::ApplicationInfo
    (
        "VKRT",
        VK_MAKE_VERSION( 1, 0, 0 ),
        "VKRT",
        VK_MAKE_VERSION( 1, 0, 0 ),
        VK_API_VERSION_1_3
    );

    auto requiredExtensions = GetRequiredExtensions();
    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo
    (
        vk::InstanceCreateFlags( vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR ),
        &applicationInfo,
        0,
        requiredExtensions.data(),
        requiredExtensions.size()
    );

    mInstance = vk::createInstance( createInfo );
}

std::vector< const char * > vks::Instance::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> requiredExtensions;
    for(uint32_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.emplace_back(glfwExtensions[i]);
    }
    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    return requiredExtensions;
}