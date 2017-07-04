//
// Created by jp on 13/06/2017.
// Description: OpenGLES 3.0 renderer for mobile devices.
//

#ifndef GRAPHICSAPI_GLES3RENDERER_H
#define GRAPHICSAPI_GLES3RENDERER_H

#include "../GraphicsAPI.h"
#include <memory>

class GLES3Renderer : public GraphicsAPI
{
public:
    GLES3Renderer();
    ~GLES3Renderer();

    void clearColour(const float r, const float g, const float b, const float a) override;
    void resizeWindow(const int width, const int height) override;
    void renderFrame(const Scene* scene) override;

private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

#endif //GRAPHICSAPI_GLES3RENDERER_H