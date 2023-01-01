//
// Created by Angelo Carly on 27/11/2022.
//

#ifndef VKRT_WINDOW_H
#define VKRT_WINDOW_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <memory>

namespace vkrt
{
    class Window
    {
        public:
            Window( int width, int height, const char * title);
            ~Window();

        public:
            vk::SurfaceKHR GetVkSurface();
            GLFWwindow * GetGLFWWindow();
        public:
            bool ShouldClose();
            void Poll();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_WINDOW_H
