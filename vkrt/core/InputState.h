//
// Created by Angelo Carly on 01/01/2023.
//

#ifndef VKRT_INPUTSTATE_H
#define VKRT_INPUTSTATE_H

#include "vks/render/ForwardDecl.h"

#include <glm/vec2.hpp>
#include <memory>

namespace vkrt
{
    class InputState
    {
        public:
            InputState( vks::WindowPtr inWindow );
            ~InputState();

        public:
            void Sync();
            void SwallowMouse();
            void ReleaseMouse();
            bool IsKeyDown( int inGLFWKey );
            bool IsButtonDown( int inGLFWButton );
            bool IsButtonClicked( int inGLFWButton );
            bool IsMouseSwallowed();
            glm::vec2 GetMouseDelta();
            glm::vec2 GetMouseScrollDelta();
            glm::vec2 GetMousePos();

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_INPUTSTATE_H
