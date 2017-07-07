//
// Created by jp on 6/07/2017.
//

#include "Object.h"

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
        // Update object position
    }

    updateTransformation();
}

void Object::updateTransformation()
{
    if (transformationUpdateRequired_)
    {
        // TODO: !!!

        // Rotate object around origin using quaternions

        // Translate object to current position

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