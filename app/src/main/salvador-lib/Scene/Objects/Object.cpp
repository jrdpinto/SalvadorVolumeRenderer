//
// Created by jp on 6/07/2017.
//

#include "Object.h"
#include "../../math/MathExtensions.h"
#include "../../Logger/Logger.h"

std::string TAG_ = "Object.cpp";

// Rotation around an axis in radians per second
static float DRAGSPEED = 40.0f;

Object::Object() : state_(State::STATIONARY), transformationUpdateRequired_(true)
{
    pos_ = {0.0f, 0.0f, 0.0f};
    quat_identity(rotation_);
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

void Object::update(float dt)
{
    if (state_ == State::MOVING)
    {
        // Update object position/orientation
        auto length = vec2_len(dragVector_.data());

        // Rotation around Y
        quat yRotation;
        vec3 yAxis = {0.0f, 1.0f, 0.0f};
        quat_rotate(yRotation, dragVector_[0]*DRAGSPEED*dt, yAxis);

        // Rotation around X
        quat xRotation;
        vec3 xAxis = {1.0f, 0.0f, 0.0f};
        quat_rotate(xRotation, -(dragVector_[1]*DRAGSPEED*dt), xAxis);

        // Concatenate rotations
        quat concatenated;
        quat_mul(concatenated, xRotation, yRotation);

        // Update current rotation
        quat oldRotation = {rotation_[0], rotation_[1], rotation_[2], rotation_[3]};
        quat_mul(rotation_, concatenated, oldRotation);

        transformationUpdateRequired_ = true;
    }

    updateTransformation();
}

void Object::updateTransformation()
{
    if (transformationUpdateRequired_)
    {
        placeAtOrigin();

        mat4x4 rotationMatrix;
        mat4x4_from_quat(rotationMatrix, rotation_);

        // Translate object to current position
        mat4x4 translationMatrix;
        mat4x4_translate(translationMatrix, -pos_[0], -pos_[1], -pos_[2]);

        // Final transformation matrix
        mat4x4_mul(transformationMatrix_, translationMatrix, rotationMatrix);

        transformationUpdateRequired_ = false;
    }
}

void Object::placeAtOrigin()
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
        dragVector_ = {0.0f, 0.0f};
        state_ = State::STATIONARY;
        transformationUpdateRequired_ = true;
    }
}

void Object::handleDragEvent(float x, float y)
{
    state_ = State::MOVING;
    dragVector_[0] = x;
    dragVector_[1] = y;
    transformationUpdateRequired_ = true;
}