//
// Created by jp on 3/07/2017.
// Description: This class acts as a container for all the objects that comprise the current scene.
// TODO: Replace with a more elaborate scene graph.
//

#ifndef SALVADOR_SCENE_H
#define SALVADOR_SCENE_H

#include <memory>

#include "Objects/Camera.h"
#include "Objects/Volume.h"
#include "../external/linmath/linmath.h"

class Scene
{
    class impl;
    std::unique_ptr<impl> pimpl_;


public:

    Scene();
    ~Scene();

    // Update all objects in the scene based on the time that has elapsed since the last frame.
    // 'elapsedTime' is assumed to be in seconds.
    void update(float elapsedTime);

    const Camera* getCamera() const;
    const Volume* getVolume() const;

    void loadVolume(std::shared_ptr<std::vector<unsigned char>> volBuffer, unsigned short width,
                              unsigned short height, unsigned short depth);

    // Touch events
    void handleDragEvent(float x, float y);
    void handleTouchUp(float x, float y);
};

#endif //SALVADOR_SCENE_H