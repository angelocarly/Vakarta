/*
 * Window abstraction
 * Manages the native window and it's images
 */

#define GLFW_INCLUDE_VULKAN

#include "vks/render/Window.h"

#include <vks/render/Instance.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "string"

// =====================================================================================================================

class vks::Window::Impl
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

vks::Window::Impl::Impl(int inWidth, int inHeight, const char * inTitle )
{
    CreateWindow( inWidth, inHeight, inTitle );
    InitializeSurface();
}

vks::Window::Impl::~Impl()
{
    vks::Instance::GetInstance().GetVkInstance().destroy( mSurface );
}

void
vks::Window::Impl::CreateWindow( int width, int height, const char * title)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->mWindow.reset( glfwCreateWindow( width, height, title, nullptr, nullptr) );

}

void
vks::Window::Impl::InitializeSurface()
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

vks::Window::Window( int width, int height, const char * title )
:
    mImpl( new Impl( width, height, title ) )
{
}

vks::Window::~Window()
{
}

// ---------------------------------------------------------------------------------------------------------------------

bool
vks::Window::ShouldClose()
{
    return glfwWindowShouldClose( mImpl->mWindow.get() );
}

void
vks::Window::Poll()
{
    glfwPollEvents();
}

vk::SurfaceKHR
vks::Window::GetVkSurface()
{
    return mImpl->mSurface;
}

GLFWwindow *
vks::Window::GetGLFWWindow()
{
    return mImpl->mWindow.get();
}
