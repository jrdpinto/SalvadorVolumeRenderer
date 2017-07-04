//
// Created by jp on 21/06/2017.
// Description: An interface that facilitates interaction with platform specific graphics APIs.
//

#ifndef GRAPHICSAPI_H
#define GRAPHICSAPI_H

#include "../Scene.h"

class GraphicsAPI
{
public:
    GraphicsAPI() {}
    virtual ~GraphicsAPI() {}

    virtual void clearColour(const float r, const float g, const float b, const float a) = 0;
    virtual void resizeWindow(const int width, const int height, const float near, const float far, const float fov) = 0;
    virtual void renderFrame(const Scene* scene) = 0;
};

#endif //GRAPHICSAPI_H
