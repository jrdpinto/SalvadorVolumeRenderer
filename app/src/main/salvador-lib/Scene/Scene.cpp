//
// Created by jp on 3/07/2017.
//

#include "Scene.h"
#include "../Logger/Logger.h"
#include <array>
#include "../math/MathExtensions.h"

const std::string TAG_ = "Scene ";

class Scene::impl
{
public:
    struct Camera
    {
        enum class State
        {
            STATIONARY,
            MOVING
        };
        State state_ = State::STATIONARY;

        std::array<float, 3> pos_ = {0.0f, 0.0f, 6.0f};

        // Orientation - By default, the camera is upright and points at the origin
        std::array<float, 3> target_ = {0.0f, 0.0f, 0.0f} ;
        std::array<float, 3> up_ = {0.0f, 1.0f, 0.0f};

        // Rotational speed (camera is currently locked to orbiting around its target)
        std::array<float, 3> rotationalSpeed_ = {0.0f, 0.0f, 0.0f};

        Scene::ProjectionParams projectionValues_ = {0.3f, 1000.0f, 45.0f};

        mat4x4 viewMatrix_;
        bool viewMatrixUpdateRequired_ = true;

        // Places camera at the origin, upright and looking down the z-axis
        void reset()
        {
            vec3 origin = {0.0f, 0.0f, 0.0f};
            vec3 target = {0.0f, 0.0f, -1.0f};
            vec3 up = {0.0f, 1.0f, 0.0f};
            mat4x4_look_at(viewMatrix_, origin, target, up);
        }
    };
    Camera cam_;

    // TODO: Add volume/list of objects here

    impl()
    {
        cam_.reset();
    }

    ~impl()
    {
    }

    void updateViewTransformation()
    {
        mat4x4_look_at(cam_.viewMatrix_, cam_.pos_.data(), cam_.target_.data(), cam_.up_.data());
        cam_.viewMatrixUpdateRequired_ = false;
    }

    void update(float elapsedTime)
    {
        // Compute view matrix only when required
        if (cam_.viewMatrixUpdateRequired_)
        {
            updateViewTransformation();
        }
    }

    void handleTouchDrag(float x, float y)
    {

    }

    void handleTouchUp(float x, float y)
    {

    }
};

Scene::Scene()
{
    pimpl_ = std::make_unique<impl>();
}

Scene::~Scene()
{

}

void Scene::setCameraPos(const float x, const float y, const float z)
{
    pimpl_->cam_.pos_ = {x,y,z};
}

void Scene::setCameraTarget(const float x, const float y, const float z)
{
    pimpl_->cam_.target_ = {x,y,z};
}

void Scene::setCameraNormal(const float x, const float y, const float z)
{
    pimpl_->cam_.up_ = {x,y,z};
}

void Scene::update(float elapsedTime)
{
    pimpl_->update(elapsedTime);
}

void Scene::handleTouchDrag(float x, float y)
{
    pimpl_->handleTouchDrag(x,y);
}

void Scene::handleTouchUp(float x, float y)
{
    pimpl_->handleTouchUp(x,y);
}

const Scene::ProjectionParams *Scene::getCameraProjectionParams() const
{
    return &(pimpl_->cam_.projectionValues_);
}

const mat4x4* Scene::getViewMatrix() const
{
    return (mat4x4 const *) &(pimpl_->cam_.viewMatrix_);
}