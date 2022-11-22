#include "vkrt/engine.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

Engine::Engine()
{
    initWindow();
    printVulkanInfo();
}

Engine::~Engine()
{
    destroyWindow();
}

// -------------------------------------------------------------------------------------------------------------------

void Engine::update()
{
    glfwPollEvents();

}

void Engine::render()
{

}

// -------------------------------------------------------------------------------------------------------------------

void Engine::initWindow()
{

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    mWindow = glfwCreateWindow( WIDTH, HEIGHT, TITLE, nullptr, nullptr);

}

void Engine::destroyWindow()
{
    glfwDestroyWindow( mWindow);
    glfwTerminate();
}

// -------------------------------------------------------------------------------------------------------------------

bool Engine::shouldClose()
{
    return glfwWindowShouldClose( mWindow);
}

// -------------------------------------------------------------------------------------------------------------------

// TODO-ENHANCEMENT: move checkerror method to a separate utility class
void checkError(VkResult result)
{
    if( result != VK_SUCCESS )
    {
        spdlog::error("err");
        throw std::runtime_error("A vulkan error occurred.");
    }
}

// TODO-ENHANCEMENT: move print vulkan information to a specific renderer class
void Engine::printVulkanInfo()
{
    uint32_t instanceVersion;
    checkError( vkEnumerateInstanceVersion(&instanceVersion) );
    uint32_t majorVersion = VK_API_VERSION_MAJOR(instanceVersion);
    uint32_t minorVersion = VK_API_VERSION_MINOR(instanceVersion);
    spdlog::info("Vulkan instance version: {}.{}", majorVersion, minorVersion);
}

