//
// Created by jp on 6/07/2017.
//

#ifndef SALVADOR_CAMERA_H
#define SALVADOR_CAMERA_H

#include "Object.h"

class Camera : public Object
{
    // Projection clipping planes
    float near_;
    float far_;

    // Field of view (degrees)
    float fov_;

    std::array<float, 3> target_;
    std::array<float,3> up_;

    void updateTransformation() override;
public:
    Camera();
    ~Camera() override;

    void setTarget(float x, float y, float z);
    void setNormal(float x, float y, float z);

    void getProjectionParams(float &near, float &far, float &fov) const;
};

#endif //SALVADOR_CAMERA_H