#include <spdlog/spdlog.h>
#include "engine.h"
#include <vulkan/vulkan.h>

Engine::Engine()
{
}

Engine::~Engine()
{

}

void Engine::update()
{
    uint32_t instanceVersion;
    auto result = vkEnumerateInstanceVersion(&instanceVersion);
    uint32_t majorVersion = VK_API_VERSION_MAJOR(instanceVersion);
    uint32_t minorVersion = VK_API_VERSION_MINOR(instanceVersion);
    spdlog::info("Vulkan instance version: {}.{}", majorVersion, minorVersion);

}

void Engine::render()
{

}
