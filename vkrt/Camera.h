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
            glm::vec3 GetForward();
            glm::vec3 GetRight();

            void SetPosition( glm::vec3 inPosition );
            void Move( glm::vec3 inDelta );
            void Forward( float inDistance );
            void Backward( float inDistance );
            void Right( float inDistance );
            void Left( float inDistance );
            void Up( float inDistance );
            void Down( float inDistance );

            glm::mat4 GetMVP();

            void RotatePitch( float inAngle );
            void RotateYaw( float inAngle );

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}


#endif //VKRT_CAMERA_H
