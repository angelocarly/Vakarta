//
// Created by magnias on 7/11/22.
//

#ifndef VKRT_ENGINE_H
#define VKRT_ENGINE_H

#include <GLFW/glfw3.h>

#include "vks/render/Instance.h"
#include "vks/render/PhysicalDevice.h"

#include "vkrt/core/ForwardDecl.h"
#include "vkrt/graphics/Renderer.h"

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
        vks::VulkanSessionPtr mVulkanSession;
        vkrt::Renderer mRenderer;
        vkrt::InputState mInputState;
        vkrt::CameraPtr mCamera;
        bool mMouseSwallowed = false;

    private:

        // Resources
        vks::AssetLoader mAssetLoader;
        std::unique_ptr< vks::Mesh > mMesh;

        // Gui
        vks::ImGuiPtr mGui;
};

#endif //VKRT_ENGINE_H
