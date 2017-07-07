//
// Created by jp on 6/07/2017.
// A high level class that defines an object in the scene. All objects have a position and an
// orientation. Objects can also respond to touch events/gestures.
//

#ifndef SALVADOR_OBJECT_H
#define SALVADOR_OBJECT_H

#include <array>

#include "../../external/linmath/linmath.h"

class Object
{
private:

protected:
    enum class State
    {
        STATIONARY,
        MOVING
    };
    State state_;

    // X, Y, Z
    std::array<float, 3> pos_;

    // X: Pitch     Y: Yaw      Z: Roll
    std::array<float, 3> orientation_;

    // Speed of movement around each axis in degrees per second
    std::array<float, 3> rotationalSpeed_ = {0.0f, 0.0f, 0.0f};

    mat4x4 transformationMatrix_;

    bool transformationUpdateRequired_;

    virtual void updateTransformation();

    // Places object at the origin, upright and looking down the z-axis
    virtual void reset();

public:
    Object();
    virtual ~Object();

    virtual void update(float dt);

    virtual void setPos(float x, float y, float z);
    virtual void setOrientation(float pitch, float yaw, float roll);

    // Input events
    virtual void handleTouchUp(float x, float y);
    virtual void handleDragEvent(float x, float y);

    const mat4x4* getTransformationMatrix() const;
};

#endif //SALVADOR_OBJECT_H