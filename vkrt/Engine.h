//
// Created by magnias on 7/11/22.
//

#ifndef VKRT_ENGINE_H
#define VKRT_ENGINE_H

#include <GLFW/glfw3.h>

#include "vks/render/Instance.h"
#include "vks/render/PhysicalDevice.h"

#include "vkrt/ForwardDecl.h"
#include "vkrt/Renderer.h"

class Engine
{

    public:
        Engine();
        ~Engine();

    public:
        void Update();
        void Render();
        bool ShouldClose();

    private:
        vks::WindowPtr mWindow;
        vkrt::Renderer mRenderer;
        vkrt::InputState mInputState;
        vkrt::CameraPtr mCamera;
        bool mMouseSwallowed = false;
};


#endif //VKRT_ENGINE_H
