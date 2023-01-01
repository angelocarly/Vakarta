/*
 * Window abstraction
 * Manages the native window and it's images
 */

#define GLFW_INCLUDE_VULKAN

#include "vkrt/Window.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <vks/Instance.h>

#include "string"

// =====================================================================================================================

class vkrt::Window::Impl
{
    public:
        Impl( int width, int height, const char * title );
        ~Impl();

        void CreateWindow( int width, int height, const char * title );
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

vkrt::Window::Impl::Impl(int inWidth, int inHeight, const char * inTitle )
{
    CreateWindow( inWidth, inHeight, inTitle );
    InitializeSurface();
}

vkrt::Window::Impl::~Impl()
{
    vks::Instance::GetInstance().GetVkInstance().destroy( mSurface );
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
        vks::Instance::GetInstance().GetVkInstance(),
        mWindow.get(),
        nullptr,
        & theVkSurface
    );
    mSurface = theVkSurface;
}


// =====================================================================================================================

vkrt::Window::Window( int width, int height, const char * title )
:
    mImpl( new Impl( width, height, title ) )
{
}

vkrt::Window::~Window()
{
}

// ---------------------------------------------------------------------------------------------------------------------

bool
vkrt::Window::ShouldClose()
{
    return glfwWindowShouldClose( mImpl->mWindow.get() );
}

void
vkrt::Window::Poll()
{
    glfwPollEvents();
}

vk::SurfaceKHR
vkrt::Window::GetVkSurface()
{
    return mImpl->mSurface;
}

GLFWwindow *
vkrt::Window::GetGLFWWindow()
{
    return mImpl->mWindow.get();
}
