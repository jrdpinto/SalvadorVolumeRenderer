//
// Created by jp on 13/06/2017.
//

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <cmath>
#include <array>
#include <string>

#include "GLES3Renderer.h"
#include "../../Logger/Logger.h"
#include "../../math/MathExtensions.h"

static const char defaultVertexShader[] =
        "#version 300 es\n"
        "uniform mat4 mvpMat;\n"
        "uniform float gradient;\n"
        "uniform float range;\n"
        "uniform vec3 closestAxis;\n"
        "in vec4 pos;\n"
        "in vec3 textureCoordUVW;\n"
        "out vec3 otextureCoordUVW;\n"
        "void main()\n"
        "{\n"
        "    // Calculate the offset for the current vertex using the instance id\n"
        "    float instance = float(gl_InstanceID);\n"
        "    float offset = ((gradient*instance)-1.0)*range;\n"
        "    float offsetZ = closestAxis.z*offset;\n"
        "    float offsetY = closestAxis.y*offset;\n"
        "    float offsetX = closestAxis.x*offset;\n"
        "\n"
        "    // Calculate the third component of the texture coordinate for this instance\n"
        "    otextureCoordUVW = textureCoordUVW;\n"
        "    otextureCoordUVW.z = ((-(pos.z+offsetZ)+range)/(2.0*range));\n"
        "\n"
        "    vec4 instancePos = vec4(pos.x+offsetX, pos.y+offsetY, pos.z+offsetZ, pos.w);\n"
        "    gl_Position = mvpMat*instancePos;\n"
        "}\n";

static const char defaultFragmentShader[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "precision lowp sampler3D;\n"
        "uniform sampler3D volumeData;\n"
        "in vec3 otextureCoordUVW;\n"
        "out vec4 outColour;\n"
        "void main()\n"
        "{\n"
        "   outColour = texture(volumeData, otextureCoordUVW);"
        "   outColour.w = outColour.x;"
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
    GLuint closestAxis_;

    // Handle to the 3d texture that is used to sample the volume
    GLuint texId_;

    // Location of the volume texture within the fragment shader
    GLuint volumeDataHandle_;

    // Holds all active buffers
    GLuint buffers_[3];

    // Prevents rendering if a buffer has not been created for the current volume
    bool bufferInitialised = false;

    impl() : eglContext_(eglGetCurrentContext()),  vtxPosHandle_(-1), mVPMatrixHandle_(-1),
             rangeHandle_(-1), gradientHandle_(-1), texCoordHandle_(-1), volumeDataHandle_(-1),
             closestAxis_(-1)
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
            texCoordHandle_ = glGetAttribLocation(defaultProgram_, "textureCoordUVW");
            mVPMatrixHandle_ = glGetUniformLocation(defaultProgram_, "mvpMat");
            rangeHandle_ = glGetUniformLocation(defaultProgram_, "range");
            gradientHandle_ = glGetUniformLocation(defaultProgram_, "gradient");
            volumeDataHandle_ = glGetUniformLocation(defaultProgram_, "volume");
            closestAxis_ = glGetUniformLocation(defaultProgram_, "closestAxis");
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
            glDeleteBuffers(3, buffers_);
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
                glGenBuffers(3, buffers_);

                glBindBuffer(GL_ARRAY_BUFFER, buffers_[2]);
                // NOTE: 'usage' is currently 'STATIC' but will need to be 'DYNAMIC' when the renderer
                // is modified to use dynamically created/clipped quads.
                auto size = sizeof(Volume::Vertex) * vol->getXYGeometry()->size();
                glBufferData(GL_ARRAY_BUFFER, size, &vol->getXYGeometry()->front(), GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, buffers_[1]);
                size = sizeof(Volume::Vertex) * vol->getXZGeometry()->size();
                glBufferData(GL_ARRAY_BUFFER, size, &vol->getXZGeometry()->front(), GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
                size = sizeof(Volume::Vertex) * vol->getYZGeometry()->size();
                glBufferData(GL_ARRAY_BUFFER, size, &vol->getYZGeometry()->front(), GL_STATIC_DRAW);

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

    // Returns a 3 element vector indicating the axis of the volume (x,y,z) that is closest to the
    // camera. Negative values indicate that the camera is behind that particular axis.
    // Also returns an integer corresponding to a vertex buffer. This buffer contains the quad
    // that is used to render geometry along the closest axis.
    int determineClosestAxis(const Scene *scene, mat4x4 *modelView, std::array<int,3>& direction)
    {
        // Determine where the camera lies in object space
        vec4 cameraInModelSpace;
        auto pos = *(scene->getCamera()->getPos());
        vec4 cameraInWorldSpace = {pos[0], pos[1], pos[2], 1.0f};
        mat4x4 inverseModelView;
        mat4x4_invert(inverseModelView,(vec4*) modelView);
        mat4x4_mul_vec4(cameraInModelSpace, inverseModelView, cameraInWorldSpace);

        vec4 cameraPosNormalised;
        vec4_norm(cameraPosNormalised, cameraInModelSpace);

        // Compute angle between camera position and each axisNo
        vec4 xAxis = {1.0f, 0.0f, 0.0f, 1.0f};
        vec4 yAxis = {0.0f, 1.0f, 0.0f, 1.0f};
        vec4 zAxis = {0.0f, 0.0f, 1.0f, 1.0f};

        std::array<float,3> axisAngles;
        axisAngles[0] = vec3_mul_inner(cameraPosNormalised, xAxis);
        axisAngles[1] = vec3_mul_inner(cameraPosNormalised, yAxis);
        axisAngles[2] = vec3_mul_inner(cameraPosNormalised, zAxis);

        // Find the axis that the camera is closest to
        auto closestAxis = std::max_element(axisAngles.begin(), axisAngles.end(),
                                            [](float i, float j)->bool{
                                                return abs(i) < abs(j);
                                            });
        int axisNo = closestAxis - axisAngles.begin();
        direction = {axisNo == 0 ? (int)std::copysign(1,*closestAxis) : 0,
                     axisNo == 1 ? (int)std::copysign(1,*closestAxis) : 0,
                     axisNo == 2 ? (int)std::copysign(1,*closestAxis) : 0};

        return axisNo;
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

            std::array<int,3> closestAxis;
            int bufferNo = determineClosestAxis(scene, &modelView_matrix, closestAxis);

            glUseProgram(defaultProgram_);

            glEnable(GL_BLEND);// Turn Blending On
            glDisable(GL_DEPTH_TEST);// Turn Depth Testing Off
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //Blending function - 'Over'

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_3D, texId_);
            glUniform1i(volumeDataHandle_, GL_TEXTURE0);

            // Activate the appropriate VBO and send data to the shader
            glBindBuffer(GL_ARRAY_BUFFER, buffers_[bufferNo]);

            glVertexAttribPointer(vtxPosHandle_, 3, GL_FLOAT, GL_FALSE, sizeof(Volume::Vertex),
                                  (const GLvoid *) offsetof(Volume::Vertex, pos_));
            glEnableVertexAttribArray(vtxPosHandle_);
            glVertexAttribPointer(texCoordHandle_, 3, GL_FLOAT, GL_FALSE, sizeof(Volume::Vertex),
                                  (const GLvoid *) offsetof(Volume::Vertex, uvw_));
            glEnableVertexAttribArray(texCoordHandle_);
            glUniformMatrix4fv(mVPMatrixHandle_, 1, GL_FALSE, *modelViewProjection_matrix);
            glUniform3f(closestAxis_, closestAxis[0], closestAxis[1], closestAxis[2]);
            glUniform1f(rangeHandle_, (scene->getVolume()->getDepthOnCurrentAxis()/2.0f));
            glUniform1f(gradientHandle_, (2.0f/(float)scene->getVolume()->getNumberOfCrossSections()));

            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0,
                                  (GLsizei) scene->getVolume()->getXYGeometry()->size(),
                                  scene->getVolume()->getNumberOfCrossSections());

            // Deactivate the VBO and texture unit
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindTexture(GL_TEXTURE_3D, 0);

            glDisable(GL_BLEND);// Turn Blending Off
            glEnable(GL_DEPTH_TEST);// Turn Depth Testing On
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