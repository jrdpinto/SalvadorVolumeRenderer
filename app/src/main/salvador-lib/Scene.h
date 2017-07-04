//
// Created by jp on 3/07/2017.
// Description: This class acts as a container for all the objects that comprise the current scene.
// TODO: Replace with a more elaborate scene graph.
//

#ifndef SALVADOR_SCENE_H
#define SALVADOR_SCENE_H

#include <array>

class Scene
{
    struct Camera
    {
        std::array<float, 3> pos_ = {0.0f, 0.0f, 20.0f};

        // Orientation - By default, the camera is upright and points at the origin
        std::array<float, 3> target_ = {0.0f, 0.0f, 0.0f} ;
        std::array<float, 3> up_ = {0.0f, 1.0f, 0.0f};

        // Projection clipping planes
        float near_ = 0.3f;
        float far_ = 1000.0f;

        // Field of view (degrees)
        float fov_ = 45.0f;
    };
    Camera cam_;

    // TODO: Add volume/list of objects here

public:
    Scene();
    ~Scene();

    // Update all objects in the scene based on the time that has elapsed since the last frame.
    // 'elapsedTime' is assumed to be in seconds.
    void update(float elapsedTime);

    void setCameraPos(const float x, const float y, const float z);
    void setCameraTarget(const float x, const float y, const float z);
    void setCameraNormal(const float x, const float y, const float z);
    const Camera* getCamera() const;
};

#endif //SALVADOR_SCENE_H