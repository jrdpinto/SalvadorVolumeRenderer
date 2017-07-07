//
// Created by jp on 6/07/2017.
//

#include "Object.h"
#include "../../math/MathExtensions.h"
#include "../../Logger/Logger.h"

std::string TAG_ = "Object.cpp";

Object::Object() : state_(State::STATIONARY), transformationUpdateRequired_(true)
{
    pos_ = {0.0f, 0.0f, 0.0f};
    orientation_ = {0.0f, 0.0f, 0.0f};
    mat4x4_identity(transformationMatrix_);
}

Object::~Object()
{
}

void Object::setPos(float x, float y, float z)
{
    pos_ = {x,y,z};
    transformationUpdateRequired_ = true;
}

void Object::setOrientation(float pitch, float yaw, float roll)
{
    orientation_ = {pitch,yaw,roll};
    transformationUpdateRequired_ = true;
}

void Object::update(float dt)
{
    if (state_ == State::MOVING)
    {
        // Update object position/orientation
        orientation_ = {orientation_[0]+(rotationalSpeed_[0]*dt),
                        orientation_[1]+rotationalSpeed_[1]*dt,
                        0.0f};
        transformationUpdateRequired_ = true;
    }

    updateTransformation();
}

void Object::updateTransformation()
{
    if (transformationUpdateRequired_)
    {
        // TODO: Debug info! Remove!
        Logger::logd(TAG_+" Calculating transform. Orientation: "+std::to_string(orientation_[0])
        +" "+std::to_string(orientation_[1])+" "+std::to_string(orientation_[2]));

        // Place object back at the origin
        reset();

        // Rotate object around origin using quaternions
        quat pitchRotation, yawRotation, compositeRotation;

        vec3 yAxis = {0.0f, 1.0f, 0.0f};
        quat_rotate(yawRotation, deg_to_rad(orientation_[1]), yAxis);

        vec3 xAxis = {1.0f, 0.0f, 0.0f};
        quat_rotate(pitchRotation, deg_to_rad(orientation_[0]), xAxis);

        // TODO: Roll around direction vector

        quat_mul(compositeRotation, pitchRotation, yawRotation);

        mat4x4 compositeRotationMatrix;
        mat4x4_from_quat(compositeRotationMatrix, compositeRotation);

        // Translate object to current position
        mat4x4 translationMatrix;
        mat4x4_translate(translationMatrix, -pos_[0], -pos_[1], -pos_[2]);

        // Final transformation matrix
        mat4x4_mul(transformationMatrix_, translationMatrix, compositeRotationMatrix);

        transformationUpdateRequired_ = false;
    }
}

void Object::reset()
{
    vec3 origin = {0.0f, 0.0f, 0.0f};
    vec3 target = {0.0f, 0.0f, -1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    mat4x4_look_at(transformationMatrix_, origin, target, up);
    transformationUpdateRequired_ = true;
}

const mat4x4 *Object::getTransformationMatrix() const
{
    return (mat4x4 const *) &transformationMatrix_;
}

void Object::handleTouchUp(float x, float y)
{
    if (state_ == State::MOVING)
    {
        state_ = State::STATIONARY;
        rotationalSpeed_ = {0.0f, 0.0f, 0.0f};
        state_ = State::STATIONARY;
        transformationUpdateRequired_ = true;
    }
}

void Object::handleDragEvent(float x, float y)
{
    state_ = State::MOVING;
    rotationalSpeed_[0] = -(400*y);
    rotationalSpeed_[1] = 400*x;
    transformationUpdateRequired_ = true;
}