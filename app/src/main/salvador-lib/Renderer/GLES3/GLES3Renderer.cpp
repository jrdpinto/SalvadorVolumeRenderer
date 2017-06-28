//
// Created by jp on 13/06/2017.
//

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>

#include "GLES3Renderer.h"
#include "../../Logger/Logger.h"

class GLES3Renderer::impl
{
    const EGLContext eglContext_;
    const std::string TAG = "GLES3Renderer";

public:
    impl() : eglContext_(eglGetCurrentContext())
    {
        if (eglContext_ != NULL)
        {
            auto glEnumToString = [](GLenum glEnum) -> std::string {return std::string((const char*) glGetString(glEnum));};
            Logger::getInstance()->log(Logger::Severity::DEBUG, TAG+" OpenGL ES environment info.");
            Logger::getInstance()->log(Logger::Severity::DEBUG, TAG+" Version: "+glEnumToString(GL_VERSION));
            Logger::getInstance()->log(Logger::Severity::DEBUG, TAG+" Vendor: "+glEnumToString(GL_VENDOR));
            Logger::getInstance()->log(Logger::Severity::DEBUG, TAG+" Renderer: "+glEnumToString(GL_RENDERER));
            Logger::getInstance()->log(Logger::Severity::DEBUG, TAG+" Extensions: "+glEnumToString(GL_EXTENSIONS));
        }
        else
        {
            Logger::getInstance()->log(Logger::Severity::ERROR, TAG+" Could not obtain handle to EGL context!");
        }
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
