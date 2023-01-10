//
// Created by Angelo Carly on 01/01/2023.
//

#include "vkrt/core/Camera.h"

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
        void RotatePitch( float ihRadians );
        void RotateYaw( float inRadians );
        void RotateRoll( float inRadians );
        void UpdateViewVectors();

    public:
        glm::vec3 mPosition;

        // Perspective values
        float mFov;
        float mAspect;
        float mZNear;
        float mZFar;

        // Individual quaternions specifying the rotation
        glm::quat mPitchQuat = glm::angleAxis( 0.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );
        glm::quat mYawQuat = glm::angleAxis( 0.0f, glm::vec3( 0.0f, 1.0f, 0.0f) );
        glm::quat mRollQuat = glm::angleAxis( 0.0f, glm::vec3( 0.0f, 0.0f, -1.0f ) );

        // Direction vectors calculated from the quaternions
        glm::vec3 mForward;
        glm::vec3 mRight;
        glm::vec3 mUp;
};

vkrt::Camera::Impl::Impl( float inFov, float inAspect, float inZNear, float inZFar )
:
    mFov( inFov ),
    mAspect( inAspect ),
    mZNear( inZNear ),
    mZFar( inZFar )
{
    UpdateViewVectors();
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
    auto theRotation = mRollQuat * mPitchQuat * mYawQuat;
//    auto theRotation = mCamQuat;
    theRotation = glm::normalize( theRotation );
    return  glm::mat4_cast( theRotation ) ;
}

glm::mat4
vkrt::Camera::Impl::CalculateProjectionMatrix() const
{
    auto thePerspective = glm::perspective(glm::radians( mFov ), mAspect, mZNear, mZFar );
    thePerspective[1][1] *= -1; // Invert the y axis (https://stackoverflow.com/questions/68508935/vulkan-default-coord-system-for-vertex-positions)
    return thePerspective;
}

/**
 * Rotate the view direction upward/downward
 * @param ihRadians
 */
void
vkrt::Camera::Impl::RotatePitch( float ihRadians )
{
    glm::quat qPitch = glm::angleAxis(ihRadians, glm::vec3( 1.0f, 0.0f, 0.0f ) );
    mPitchQuat = glm::normalize( qPitch * mPitchQuat );
    UpdateViewVectors();
}

/**
 * Rotate the view direction left/right
 * @param inRadians
 */
void
vkrt::Camera::Impl::RotateYaw( float inRadians )
{
    glm::quat qYaw = glm::angleAxis(inRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    mYawQuat = glm::normalize( mYawQuat * qYaw );
    UpdateViewVectors();
}

/**
 * Rotate the view direction around forward axis
 * @param inRadians
 */
void
vkrt::Camera::Impl::RotateRoll( float inRadians )
{
    glm::quat qRoll = glm::angleAxis(inRadians, glm::vec3( 0.0f, 0.0f, -1.0f ) );
    mRollQuat = glm::normalize( qRoll * mRollQuat );
    UpdateViewVectors();
}

void vkrt::Camera::Impl::UpdateViewVectors()
{
    auto theRotation = glm::inverse( mPitchQuat * mRollQuat * mYawQuat );
//    auto theRotation = glm::inverse( mCamQuat );
    mRight = glm::normalize( theRotation * glm::vec3(1, 0, 0));
    mUp = glm::normalize( theRotation * glm::vec3(0, 1, 0));
    mForward = glm::normalize( theRotation * glm::vec3(0, 0, -1)); // Left-handed coordinates
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

void
vkrt::Camera::Move( glm::vec3 inDelta )
{
    mImpl->mPosition += inDelta;
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

void
vkrt::Camera::RotateRoll( float inAngle )
{
    mImpl->RotateRoll( inAngle );
}

glm::vec3
vkrt::Camera::GetForward()
{
    return mImpl->mForward;
}

glm::vec3
vkrt::Camera::GetRight()
{
    return mImpl->mRight;
}

void
vkrt::Camera::Forward( float inDistance )
{
    mImpl->mPosition += mImpl->mForward * inDistance;
}

void
vkrt::Camera::Backward( float inDistance )
{
    mImpl->mPosition -= mImpl->mForward * inDistance;
}

void
vkrt::Camera::Right( float inDistance )
{
    mImpl->mPosition += mImpl->mRight * inDistance;
}

void
vkrt::Camera::Left( float inDistance )
{
    mImpl->mPosition -= mImpl->mRight * inDistance;
}

void
vkrt::Camera::Up( float inDistance )
{
    mImpl->mPosition += mImpl->mUp * inDistance;
}

void
vkrt::Camera::Down( float inDistance )
{
    mImpl->mPosition -= mImpl->mUp * inDistance;
}
