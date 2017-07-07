//
// Created by jp on 6/07/2017.
//

#include "Camera.h"

Camera::Camera() : near_(0.3f), far_(1000.0f), fov_(45.0f)
{
    target_ = {0.0f, 0.0f, -1.0f};
    up_ = {0.0f, 1.0f, 0.0f};
}

Camera::~Camera()
{
}

void Camera::updateTransformation()
{
    // TODO: Camera is currently locked to a target. Implement free-look.
    if (transformationUpdateRequired_)
    {
        mat4x4_look_at(transformationMatrix_, pos_.data(), target_.data(), up_.data());
        transformationUpdateRequired_ = false;
    }
}

void Camera::setTarget(float x, float y, float z)
{
    target_ = {x,y,z};
    transformationUpdateRequired_ = true;
}

void Camera::setNormal(float x, float y, float z)
{
    up_ = {x,y,z};
}

void Camera::getProjectionParams(float &near, float &far, float &fov) const
{
    near = near_;
    far = far_;
    fov = fov_;
}