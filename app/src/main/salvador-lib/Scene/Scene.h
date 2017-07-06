//
// Created by jp on 3/07/2017.
// Description: This class acts as a container for all the objects that comprise the current scene.
// TODO: Replace with a more elaborate scene graph.
//

#ifndef SALVADOR_SCENE_H
#define SALVADOR_SCENE_H

#include <memory>
#include "../external/linmath/linmath.h"

class Scene
{
    class impl;
    std::unique_ptr<impl> pimpl_;

public:

    Scene();
    ~Scene();

    struct ProjectionParams
    {
        // Projection clipping planes
        float near_ = 0.0f;
        float far_ = 0.0f;

        // Field of view (degrees)
        float fov_ = 0.0f;
    };

    // Update all objects in the scene based on the time that has elapsed since the last frame.
    // 'elapsedTime' is assumed to be in seconds.
    void update(float elapsedTime);

    // Camera interface
    void setCameraPos(const float x, const float y, const float z);
    void setCameraTarget(const float x, const float y, const float z);
    void setCameraNormal(const float x, const float y, const float z);

    const ProjectionParams* getCameraProjectionParams() const;
    const mat4x4* getViewMatrix() const;

    void handleTouchDrag(float x, float y);
    void handleTouchUp(float x, float y);
};

#endif //SALVADOR_SCENE_H