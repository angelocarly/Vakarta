//
// Created by Angelo Carly on 27/11/2022.
//

#ifndef VKRT_WINDOW_H
#define VKRT_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <memory>

namespace vkrt
{
    class Window
    {
        public:
            Window( int width, int height, const char * title);
            ~Window();

            bool ShouldClose();
            void Poll();
            vk::SurfaceKHR CreateSurface();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_WINDOW_H
