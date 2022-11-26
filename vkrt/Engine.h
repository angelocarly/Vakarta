//
// Created by magnias on 7/11/22.
//

#ifndef VKRT_ENGINE_H
#define VKRT_ENGINE_H

#include <GLFW/glfw3.h>

#include "vks/Instance.h"
#include "vks/PhysicalDevice.h"

#include "vkrt/Renderer.h"

class Engine
{

    public:

        Engine();
        ~Engine();

        void update();
        void render();

        bool shouldClose();

    private:
        void initWindow();
        void destroyWindow();
        void printVulkanInfo();

        GLFWwindow* mWindow;
        vkrt::Renderer mRenderer;

};


#endif //VKRT_ENGINE_H
