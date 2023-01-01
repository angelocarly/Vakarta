//
// Created by Angelo Carly on 01/01/2023.
//

#ifndef VKRT_CAMERA_H
#define VKRT_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

namespace vkrt
{
    class Camera
    {
        public:
            Camera( float inFov, float inAspect, float inZNear, float inZFar );
            ~Camera();
        public:
            glm::vec3 GetPosition();
            void SetPosition( glm::vec3 inPosition );

            glm::mat4 GetMVP();

            void RotatePitch( float inAngle );
            void RotateYaw( float inAngle );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_CAMERA_H
