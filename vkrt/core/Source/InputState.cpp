//
// Created by Angelo Carly on 01/01/2023.
//

#include "vkrt/core/InputState.h"

#include "vks/render/Window.h"

#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>

class vkrt::InputState::Impl
{
    public:
        Impl( vks::WindowPtr inWindow );
        ~Impl();

    public:
        static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods );
        static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods );
        static void mouse_cursor_callback( GLFWwindow *window, double xpos, double ypos );

    public:
        void Sync();

    public:
        vks::WindowPtr mWindow;

    public:
        static const bool kEnableImGuiCallbacks = false;

    public:
        static bool mKeys[GLFW_KEY_LAST];
        glm::vec2 mMousePos;
        static glm::vec2 mMousePosBuffer;
        glm::vec2 mMouseDelta;
        static glm::vec2 mMouseDeltaBuffer;

        int mButtons[GLFW_MOUSE_BUTTON_LAST];
        static int mButtonsBuffer[GLFW_MOUSE_BUTTON_LAST];
        bool mMouseSwallowed;
};

vkrt::InputState::Impl::Impl( vks::WindowPtr inWindow )
:
    mWindow( inWindow )
{
    glfwSetKeyCallback( mWindow->GetGLFWWindow(), key_callback);
    glfwSetMouseButtonCallback(mWindow->GetGLFWWindow(), mouse_button_callback);
    glfwSetCursorPosCallback(mWindow->GetGLFWWindow(), mouse_cursor_callback);

    mMousePos = glm::vec2();
    mMouseDelta = glm::vec2();

}

vkrt::InputState::Impl::~Impl()
{

}

// Redefine callback variables for linker
bool vkrt::InputState::Impl::mKeys[GLFW_KEY_LAST];
glm::vec2 vkrt::InputState::Impl::mMousePosBuffer;
glm::vec2 vkrt::InputState::Impl::mMouseDeltaBuffer;
int vkrt::InputState::Impl::mButtonsBuffer[GLFW_MOUSE_BUTTON_LAST];

void
vkrt::InputState::Impl::key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
    if( kEnableImGuiCallbacks )
        ImGui_ImplGlfw_KeyCallback( window, key, scancode, action, mods );

    if (key >= 0 && key < GLFW_KEY_LAST) mKeys[key] = action != GLFW_RELEASE;
}

void
vkrt::InputState::Impl::mouse_button_callback( GLFWwindow *window, int button, int action, int mods )
{
    if( kEnableImGuiCallbacks )
        ImGui_ImplGlfw_MouseButtonCallback( window, button, action, mods );

    if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) mButtonsBuffer[button] = action;
}

void
vkrt::InputState::Impl::mouse_cursor_callback(GLFWwindow *window, double xpos, double ypos )
{
    if( kEnableImGuiCallbacks )
        ImGui_ImplGlfw_CursorPosCallback( window, xpos, ypos );

    glm::vec2 prevMousePos = mMousePosBuffer;
    mMousePosBuffer.x = (float) xpos;
    mMousePosBuffer.y = (float) ypos;

    glm::vec2 delta = mMousePosBuffer - prevMousePos;
    mMouseDeltaBuffer += delta;
}

void
vkrt::InputState::Impl::Sync()
{
    mMousePos = mMousePosBuffer;

    mMouseDelta = mMouseDeltaBuffer;
    mMouseDeltaBuffer = glm::vec2();

    for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
    {
        mButtons[i] = mButtonsBuffer[i];
        mButtonsBuffer[i] = 0;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

vkrt::InputState::InputState( vks::WindowPtr inWindow )
:
    mImpl( new Impl( inWindow ) )
{

}

vkrt::InputState::~InputState()
{

}

/**
 * Synchronize all callback data with buffered data
 */
void
vkrt::InputState::Sync()
{
    mImpl->Sync();
}

bool
vkrt::InputState::IsKeyDown( int inGLFWKey )
{
    return mImpl->mKeys[ inGLFWKey ];
}

bool
vkrt::InputState::IsButtonDown( int inGLFWButton )
{
    return mImpl->mButtons[ inGLFWButton ];
}

bool
vkrt::InputState::IsButtonClicked( int inGLFWButton )
{
    return mImpl->mButtons[ inGLFWButton ] == GLFW_PRESS;
}

glm::vec2
vkrt::InputState::GetMouseDelta()
{
    return mImpl->mMouseDelta;
}

glm::vec2
vkrt::InputState::GetMousePos()
{
    return mImpl->mMousePos;
}

void
vkrt::InputState::SwallowMouse()
{
    glfwSetInputMode( mImpl->mWindow->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwSetInputMode( mImpl->mWindow->GetGLFWWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE );
    mImpl->mMouseSwallowed = true;
}

void
vkrt::InputState::ReleaseMouse()
{
    glfwSetInputMode( mImpl->mWindow->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    glfwSetInputMode( mImpl->mWindow->GetGLFWWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE );
    mImpl->mMouseSwallowed = false;
}

bool
vkrt::InputState::IsMouseSwallowed()
{
    return mImpl->mMouseSwallowed;
}

