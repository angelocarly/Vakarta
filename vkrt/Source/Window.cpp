//
// Created by Angelo Carly on 27/11/2022.
//

#include "vkrt/Window.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <vks/Instance.h>

#include "string"

// =====================================================================================================================

class vkrt::Window::Impl
{
    public:
        Impl();
        ~Impl();

        void CreateWindow( int widht, int height, const char * title );
        void InitializeSurface();

        struct DestroyWindow
        {
            void operator()( GLFWwindow * ptr )
            {
                glfwDestroyWindow( ptr );
                glfwTerminate();
            }
        };

    public:
        std::unique_ptr< GLFWwindow, DestroyWindow > mWindow;
        vk::SurfaceKHR mSurface;
};

vkrt::Window::Impl::Impl()
{

}

vkrt::Window::Impl::~Impl()
{

}

void
vkrt::Window::Impl::CreateWindow( int width, int height, const char * title)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->mWindow.reset( glfwCreateWindow( width, height, title, nullptr, nullptr) );

}

void
vkrt::Window::Impl::InitializeSurface()
{
    VkSurfaceKHR theVkSurface;
    glfwCreateWindowSurface
    (
        vks::Instance::GetInstance().GetVulkanInstance(),
        mWindow.get(),
        nullptr,
        & theVkSurface
    );
    mSurface = theVkSurface;
}


// =====================================================================================================================

vkrt::Window::Window( int width, int height, const char * title )
:
    mImpl( new Impl() )
{
    mImpl->CreateWindow( width, height, title );
    mImpl->InitializeSurface();
}

vkrt::Window::~Window()
{
    vks::Instance::GetInstance().GetVulkanInstance().destroy( mImpl->mSurface );
}

// ---------------------------------------------------------------------------------------------------------------------

bool vkrt::Window::ShouldClose()
{
    return glfwWindowShouldClose( mImpl->mWindow.get() );
}

void vkrt::Window::Poll()
{
    glfwPollEvents();
}

vk::SurfaceKHR vkrt::Window::GetSurface()
{
    return mImpl->mSurface;
}

