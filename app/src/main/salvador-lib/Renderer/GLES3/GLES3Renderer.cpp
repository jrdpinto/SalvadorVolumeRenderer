//
// Created by jp on 13/06/2017.
//

#include "GLES3Renderer.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>

class GLES3Renderer::impl
{
    const EGLContext eglContext_;

public:
    impl() : eglContext_(eglGetCurrentContext())
    {
    }

    ~impl()
    {
        if (eglContext_ == eglGetCurrentContext())
        {
            // TODO: Use context to free resources
        }
    }
};

GLES3Renderer::GLES3Renderer()
{
    pimpl_ = std::make_unique<impl>();
}

GLES3Renderer::~GLES3Renderer()
{
}

void GLES3Renderer::clearColour(const float r, const float g, const float b, const float a)
{
    glClearColor(r, g, b, a);
}

void GLES3Renderer::resizeWindow(const float width, const float height)
{
    glViewport(0, 0, width, height);
}

void GLES3Renderer::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
