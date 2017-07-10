//
// Created by jp on 13/06/2017.
//

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>

#include "GLES3Renderer.h"
#include "../../Logger/Logger.h"
#include "../../math/MathExtensions.h"

static const char defaultVertexShader[] =
        "#version 300 es\n"
        "uniform mat4 mvpMat;\n"
        "uniform float gradient;\n"
        "uniform float range;\n"
        "in vec4 pos;\n"
        "in vec2 textureCoordUV;\n"
        "out vec3 textureCoordUVW;\n"
        "void main()\n"
        "{\n"
        "    // Calculate the offset for the current vertex using the instance id\n"
        "    float instance = float(gl_InstanceID);\n"
        "    float offset = ((gradient*instance)-1.0)*range;\n"
        "\n"
        "    // Calculate the third component of the texture coordinate for this instance\n"
        "    textureCoordUVW.xy = textureCoordUV;\n"
        "    textureCoordUVW.z = 0.5;\n"
        "\n"
        "    vec4 instancePos = vec4(pos.x, pos.y, pos.z+offset, pos.w);\n"
        "    gl_Position = mvpMat*instancePos;\n"
        "}\n";

static const char defaultFragmentShader[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "precision lowp sampler3D;\n"
        "uniform sampler3D volumeData;\n"
        "in vec3 textureCoordUVW;\n"
        "out vec4 outColour;\n"
        "void main()\n"
        "{\n"
        //"    outColour.xyz = textureCoordUVW;\n"
        //"    outColour.w = 1.0;\n"
        "   outColour = texture(volumeData, textureCoordUVW);"
        "}\n";

static mat4x4 projection_matrix;
static mat4x4 view_projection_matrix;
static mat4x4 model_view_projection_matrix;

class GLES3Renderer::impl
{
public:
    const EGLContext eglContext_;
    const std::string TAG_ = "GLES3Renderer";

    // Handles to the shader programs and its inputs
    GLuint defaultProgram_;
    GLuint vtxPosHandle_;
    GLuint mVPMatrixHandle_;
    GLuint rangeHandle_;
    GLuint gradientHandle_;
    GLuint texCoordHandle_;

    // Handle to the 3d texture that is used to sample the volume
    GLuint texId_;

    // Location of the volume texture within the fragment shader
    GLuint volumeDataHandle_;

    // Holds all active buffers
    GLuint buffers_[1];

    // Prevents rendering if a buffer has not been created for the current volume
    bool bufferInitialised = false;

    impl() : eglContext_(eglGetCurrentContext()),  vtxPosHandle_(-1), mVPMatrixHandle_(-1),
             rangeHandle_(-1), gradientHandle_(-1), texCoordHandle_(-1), volumeDataHandle_(-1)
    {
        if (eglContext_ != NULL)
        {
            outputEnvInfo();

            mat4x4_identity(projection_matrix);
            mat4x4_identity(view_projection_matrix);
            mat4x4_identity(model_view_projection_matrix);

            glEnable(GL_DEPTH_TEST);

            // Load and initialise default shaders
            Logger::logd(TAG_ + " Initialising default shaders.");
            defaultProgram_ = createProgram(defaultVertexShader, defaultFragmentShader);
            vtxPosHandle_ = glGetAttribLocation(defaultProgram_, "pos");
            texCoordHandle_ = glGetAttribLocation(defaultProgram_, "textureCoordUV");
            mVPMatrixHandle_ = glGetUniformLocation(defaultProgram_, "mvpMat");
            rangeHandle_ = glGetUniformLocation(defaultProgram_, "range");
            gradientHandle_ = glGetUniformLocation(defaultProgram_, "gradient");
            volumeDataHandle_ = glGetUniformLocation(defaultProgram_, "volume");
        }
        else
        {
            Logger::loge(TAG_ + " Could not obtain handle to EGL context!");
        }
    }

    ~impl()
    {
        if (eglContext_ == eglGetCurrentContext())
        {
            // Free all resources
            glDeleteProgram(defaultProgram_);
            Logger::logd(TAG_+" Deleting buffers.");
            glDeleteBuffers(1, buffers_);
            glDeleteTextures(1, &texId_);
        }
    }

    bool checkGlError(const char* funcName)
    {
        bool error = false;

        GLint errorCode = glGetError();
        if (errorCode != GL_NO_ERROR)
        {
            Logger::loge(TAG_ + " GL error after " + funcName + " Error#: " + std::to_string(errorCode));
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
                    Logger::loge(TAG_ + " Could not compile " +
                    (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") + " shader.");
                    Logger::loge(TAG_ + " Info:\n" + infoLog);
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
                    Logger::loge(TAG_ + " Could not link program.");
                    GLint infoLogLen = 0;
                    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLen);
                    if (infoLogLen)
                    {
                        GLchar infoLog[infoLogLen];
                        glGetProgramInfoLog(programId, infoLogLen, NULL, infoLog);
                        Logger::loge(TAG_ + " Info:\n" + infoLog);
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
        Logger::logd(TAG_ + " OpenGL ES environment info.");
        Logger::logd(TAG_ + " Version: " + glEnumToString(GL_VERSION));
        Logger::logd(TAG_ + " Vendor: " + glEnumToString(GL_VENDOR));
        Logger::logd(TAG_ + " Renderer: " + glEnumToString(GL_RENDERER));
        Logger::logd(TAG_ + " Extensions: " + glEnumToString(GL_EXTENSIONS));
        int maxTextureSize;
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &maxTextureSize);
        Logger::logd(TAG_ + " Max 3D texture size: " + std::to_string(maxTextureSize));
    }

    // Initialises all required buffers for the current volume
    void initBuffers(const Scene* scene)
    {
        if (!bufferInitialised)
        {
            auto vol = scene->getVolume();
            if (vol != nullptr)
            {
                // Initialise vertex buffer(s)
                Logger::logd(TAG_+" Initialising vertex buffers.");
                glGenBuffers(1, buffers_);
                glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
                // NOTE: 'usage' is currently 'STATIC' but will need to be 'DYNAMIC' when the renderer
                // is modified to use dynamically created/clipped quads.
                auto size = sizeof(Volume::Vertex) * vol->getGeometry()->size();
                glBufferData(GL_ARRAY_BUFFER, size, &vol->getGeometry()->front(), GL_STATIC_DRAW);

                // Initialise 3D texture
                Logger::logd(TAG_+" Initialising 3D texture.");
                glGenTextures(1, &texId_);
                glBindTexture(GL_TEXTURE_3D, texId_);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                float width, height, depth;
                vol->getDimensions(width, height, depth);
                glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, width, height, depth, 0, GL_LUMINANCE,
                             GL_UNSIGNED_BYTE, &(vol->getTextureData()->front()));
                glBindTexture(GL_TEXTURE_3D, 0);

                bufferInitialised = true;
            }
        }
    }

    void renderFrame(const Scene* scene)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        initBuffers(scene);

        if (bufferInitialised)
        {
            mat4x4 modelView_matrix;
            mat4x4_mul(modelView_matrix, (vec4 *) scene->getCamera()->getTransformationMatrix(),
                                         (vec4 *) scene->getVolume()->getTransformationMatrix());

            mat4x4 modelViewProjection_matrix;
            mat4x4_mul(modelViewProjection_matrix, projection_matrix, modelView_matrix);

            glUseProgram(defaultProgram_);

            glEnable(GL_TEXTURE_3D);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_3D, texId_);
            glUniform1i(volumeDataHandle_, GL_TEXTURE0);

            // Activate the VBO and send data to the shader
            glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
            glVertexAttribPointer(vtxPosHandle_, 3, GL_FLOAT, GL_FALSE, sizeof(Volume::Vertex),
                                  (const GLvoid *) offsetof(Volume::Vertex, pos_));
            glEnableVertexAttribArray(vtxPosHandle_);
            glVertexAttribPointer(texCoordHandle_, 2, GL_FLOAT, GL_FALSE, sizeof(Volume::Vertex),
                                  (const GLvoid *) offsetof(Volume::Vertex, uv_));
            glEnableVertexAttribArray(texCoordHandle_);
            glUniformMatrix4fv(mVPMatrixHandle_, 1, GL_FALSE, *modelViewProjection_matrix);
            glUniform1f(rangeHandle_, (scene->getVolume()->getDepthOnCurrentAxis()/2.0f));
            glUniform1f(gradientHandle_, (2.0f/(float)scene->getVolume()->getNumberOfCrossSections()));

            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0,
                                  (GLsizei) scene->getVolume()->getGeometry()->size(),
                                  scene->getVolume()->getNumberOfCrossSections());

            // Deactivate the VBO and texture unit
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindTexture(GL_TEXTURE_3D, 0);
            glDisable(GL_TEXTURE_3D);
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

void GLES3Renderer::resizeWindow(const int width, const int height, const float near, const float far, const float fov)
{
    glViewport(0, 0, width, height);

    // Adjust the projection transformation to account for the new screen size
    mat4x4_perspective(projection_matrix, deg_to_rad(fov), (float)width/(float)height, near, far);
}

void GLES3Renderer::renderFrame(const Scene* scene)
{
    pimpl_->renderFrame(scene);
}