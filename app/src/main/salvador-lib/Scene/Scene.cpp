//
// Created by jp on 3/07/2017.
//

#include <array>

#include "Scene.h"
#include "../Logger/Logger.h"
#include "../math/MathExtensions.h"

const std::string TAG_ = "Scene ";

class Scene::impl
{
public:
    Camera cam_;
    Volume vol_;

    impl()
    {
        cam_.setPos(0.0f, 0.0f, 6);
        cam_.setTarget(0.0f, 0.0f, -1.0f);
    }

    ~impl()
    {
    }

    void update(float elapsedTime)
    {
        cam_.update(elapsedTime);
        vol_.update(elapsedTime);
    }
};

Scene::Scene()
{
    pimpl_ = std::make_unique<impl>();
}

Scene::~Scene()
{

}

void Scene::update(float elapsedTime)
{
    pimpl_->update(elapsedTime);
}

void Scene::handleDragEvent(float x, float y)
{
    pimpl_->vol_.handleDragEvent(x, y);
}

void Scene::handleTouchUp(float x, float y)
{
    pimpl_->vol_.handleTouchUp(x, y);
}

const Camera *Scene::getCamera() const
{
    return &pimpl_->cam_;
}

void Scene::addVolume()
{

}

const Volume *Scene::getVolume() const
{
    return &pimpl_->vol_;
}