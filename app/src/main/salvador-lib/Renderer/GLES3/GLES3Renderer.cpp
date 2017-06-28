//
// Created by jp on 13/06/2017.
//

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>

#include "GLES3Renderer.h"
#include "../../Logger/Logger.h"

static const char defaultVertexShader[] =
        "#version 300 es\n"
        "in vec4 pos;\n"
        "in vec4 colour;\n"
        "out vec4 vColour;\n"
        "void main()\n"
        "{\n"
        "    //TODO: Transform gl_Position into projection space\n"
        "    gl_Position = pos;\n"
        "    vColour = colour;\n"
        "}\n";

static const char defaultFragmentShader[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec4 vColour;\n"
        "out vec4 outColour;\n"
        "void main()\n"
        "{\n"
        "    outColour = vColour;\n"
        "}\n";

class GLES3Renderer::impl
{
public:
    const EGLContext eglContext_;
    const std::string TAG_ = "GLES3Renderer";
    GLuint defaultProgram_;

    bool checkGlError(const char* funcName)
    {
        bool error = false;

        GLint errorCode = glGetError();
        if (errorCode != GL_NO_ERROR)
        {
            Logger::getInstance()->log(Logger::Severity::ERROR, TAG_+" GL error after "+funcName+" Error#: "+std::to_string(errorCode));
            error = true;
        }

        return error;
    }

    GLuint compileShader(GLuint shaderType, const char* shaderSource)
    {
        GLuint shader = glCreateShader(shaderType);
        if (!shader)
        {
            checkGlError("compileShader()");
        }
        else
        {
            GLint compiled = GL_FALSE;
            glShaderSource(shader, 1, &shaderSource, NULL);
            glCompileShader(shader);
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

            if (!compiled)
            {
                GLint infoLogLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
                if (infoLogLen > 0)
                {
                    GLchar infoLog[infoLogLen];
                    glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                    Logger::getInstance()->log(Logger::Severity::ERROR, TAG_+
                            " Could not compile "+(shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")+" shader.");
                    Logger::getInstance()->log(Logger::Severity::ERROR, TAG_+" Info:\n"+infoLog);
                }

                glDeleteShader(shader);
                shader = 0;
            }
        }

        return shader;
    }

    GLuint createProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
    {
        GLuint programId = 0;
        GLuint vertexShader, fragmentShader;
        GLint linked = GL_FALSE;

        vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        if (vertexShader != 0 && fragmentShader != 0)
        {
            programId = glCreateProgram();
            if (!programId)
            {
                checkGlError("createProgram");
            }
            else
            {
                glAttachShader(programId, vertexShader);
                glAttachShader(programId, fragmentShader);
                glLinkProgram(programId);
                glGetProgramiv(programId, GL_LINK_STATUS, &linked);

                if (!linked)
                {
                    Logger::getInstance()->log(Logger::Severity::ERROR, TAG_+" Could not link program.");
                    GLint infoLogLen = 0;
                    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLen);
                    if (infoLogLen)
                    {
                        GLchar infoLog[infoLogLen];
                        glGetProgramInfoLog(programId, infoLogLen, NULL, infoLog);
                        Logger::getInstance()->log(Logger::Severity::ERROR, TAG_+" Info:\n"+infoLog);
                    }

                    glDeleteProgram(programId);
                    programId = 0;
                }
            }
        }

        // Flag shaders for deletion so that they are released when the program is deleted
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return programId;
    }

    void outputEnvInfo()
    {
        auto glEnumToString = [](GLenum glEnum) -> std::string {return std::string((const char*) glGetString(glEnum));};
        Logger::getInstance()->log(Logger::Severity::DEBUG, TAG_+" OpenGL ES environment info.");
        Logger::getInstance()->log(Logger::Severity::DEBUG, TAG_+" Version: "+glEnumToString(GL_VERSION));
        Logger::getInstance()->log(Logger::Severity::DEBUG, TAG_+" Vendor: "+glEnumToString(GL_VENDOR));
        Logger::getInstance()->log(Logger::Severity::DEBUG, TAG_+" Renderer: "+glEnumToString(GL_RENDERER));
        Logger::getInstance()->log(Logger::Severity::DEBUG, TAG_+" Extensions: "+glEnumToString(GL_EXTENSIONS));
    }

    impl() : eglContext_(eglGetCurrentContext())
    {
        if (eglContext_ != NULL)
        {
            outputEnvInfo();

            // Load and initialise default shaders
            Logger::getInstance()->log(Logger::Severity::DEBUG, TAG_+" Loading default shaders.");
            defaultProgram_ = createProgram(defaultVertexShader, defaultFragmentShader);
        }
        else
        {
            Logger::getInstance()->log(Logger::Severity::ERROR, TAG_+" Could not obtain handle to EGL context!");
        }
    }

    ~impl()
    {
        if (eglContext_ == eglGetCurrentContext())
        {
            // Free all compiled programs
            glDeleteProgram(defaultProgram_);
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

    // TODO: Load the appropriate shader program. Draw the current vertex buffer.
    glUseProgram(pimpl_->defaultProgram_);
}