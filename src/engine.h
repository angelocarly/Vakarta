//
// Created by magnias on 7/11/22.
//

#ifndef VKRT_ENGINE_H
#define VKRT_ENGINE_H


#include <GLFW/glfw3.h>
#include "vks/vks_device.h"

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

    GLFWwindow* window;
    VksDevice device;

};


#endif //VKRT_ENGINE_H
