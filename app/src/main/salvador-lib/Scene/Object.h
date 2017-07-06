//
// Created by jp on 6/07/2017.
// A high level class that defines an object in the scene. All objects have a position and an
// orientation. Objects can also respond to touch events/gestures.
//

#ifndef SALVADOR_OBJECT_H
#define SALVADOR_OBJECT_H

#include <array>

#include "../external/linmath/linmath.h"

class Object
{
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

    mat4x4 transformationMatrix_;

    bool transformationUpdateRequired_;

    void updateTransformation();
public:
    Object();
    virtual ~Object();

    const mat4x4* getViewMatrix() const;

    virtual void update(float dt);

    virtual void setPos(float x, float y, float z);
    virtual void setOrientation(float pitch, float yaw, float roll);
};

#endif //SALVADOR_OBJECT_H