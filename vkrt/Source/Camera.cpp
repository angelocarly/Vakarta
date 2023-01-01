//
// Created by Angelo Carly on 01/01/2023.
//

#include "vkrt/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class vkrt::Camera::Impl
{
    public:
        Impl( float inFov, float inAspect, float inZNear, float inZFar );
        ~Impl();

    public:
        glm::mat4 CalculateModelMatrix() const;
        glm::mat4 CalculateViewMatrix() const;
        glm::mat4 CalculateProjectionMatrix() const;
        void RotatePitch( float rads );
        void RotateYaw( float rads );

    public:
        glm::vec3 mPosition;
        float mFov;
        float mAspect;
        float mZNear;
        float mZFar;

        glm::quat mYawQuat;
        glm::quat mPitchQuat;
        glm::quat mRollQuat;
};

vkrt::Camera::Impl::Impl( float inFov, float inAspect, float inZNear, float inZFar )
:
    mFov( inFov ),
    mAspect( inAspect ),
    mZNear( inZNear ),
    mZFar( inZFar )
{

}

vkrt::Camera::Impl::~Impl()
{

}

glm::mat4
vkrt::Camera::Impl::CalculateModelMatrix() const
{
    return glm::translate( glm::mat4( 1.0f ), -mPosition );
}

glm::mat4
vkrt::Camera::Impl::CalculateViewMatrix() const
{
    auto theRotation = mYawQuat * mPitchQuat * mRollQuat;
    glm::normalize( theRotation );
    return glm::mat4_cast( theRotation );
}

glm::mat4
vkrt::Camera::Impl::CalculateProjectionMatrix() const
{
    return glm::perspective(glm::radians( mFov ), mAspect, mZNear, mZFar );
}

/**
 * Rotate the view direction upward/downward
 * @param rads
 */
void
vkrt::Camera::Impl::RotatePitch( float rads )
{
    glm::quat qPitch = glm::angleAxis(rads, glm::vec3(1.0f, 0, 0));
    mPitchQuat = mPitchQuat * qPitch;
}

/**
 * Rotate the view direction left/right
 * @param rads
 */
void
vkrt::Camera::Impl::RotateYaw( float rads ) // rotate around cams local X axis
{
    glm::quat qYaw = glm::angleAxis(rads, glm::vec3(0.0f, 1.0f, 0));
    mYawQuat = mYawQuat * qYaw;
}

// ---------------------------------------------------------------------------------------------------------------------

vkrt::Camera::Camera( float inFov, float inAspect, float inZNear, float inZFar )
:
    mImpl( new Impl( inFov, inAspect, inZNear, inZFar ) )
{

}

vkrt::Camera::~Camera()
{

}

glm::vec3
vkrt::Camera::GetPosition()
{
    return mImpl->mPosition;
}

void
vkrt::Camera::SetPosition( glm::vec3 inPosition )
{
    mImpl->mPosition = inPosition;
}

glm::mat4
vkrt::Camera::GetMVP()
{
    return mImpl->CalculateProjectionMatrix() * mImpl->CalculateViewMatrix() * mImpl->CalculateModelMatrix();
}

void
vkrt::Camera::RotatePitch( float inAngle )
{
    mImpl->RotatePitch( inAngle );
}

void
vkrt::Camera::RotateYaw( float inAngle )
{
    mImpl->RotateYaw( inAngle );
}
