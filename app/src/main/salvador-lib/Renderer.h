//
// Created by jp on 24/06/2017.
//

#ifndef VOLUMERENDERER_RENDERER_H
#define VOLUMERENDERER_RENDERER_H

#include <memory>

#include "GraphicsAPI.h"

class Renderer
{
public:
    Renderer(std::shared_ptr<GraphicsAPI> api);
    ~Renderer();

    void resizeWindow(const int width, const int height);
    void renderFrame();

private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

#endif //VOLUMERENDERER_RENDERER_H