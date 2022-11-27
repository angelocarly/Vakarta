//
// Created by magnias on 7/11/22.
//

#ifndef VKRT_ENGINE_H
#define VKRT_ENGINE_H

#include <GLFW/glfw3.h>

#include "vks/Instance.h"
#include "vks/PhysicalDevice.h"

#include "vkrt/Renderer.h"
#include "Window.h"

class Engine
{

    public:
        Engine();
        ~Engine();

        void Update();
        void Render();
        bool ShouldClose();
        vk::SurfaceKHR GetSurface();

    private:
        vkrt::Window mWindow;
        vkrt::Renderer mRenderer;

};


#endif //VKRT_ENGINE_H
