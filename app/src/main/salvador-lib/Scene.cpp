//
// Created by jp on 3/07/2017.
//

#include "Scene.h"
#include "Logger/Logger.h"

const std::string TAG_ = "Scene ";

Scene::Scene()
{
}

Scene::~Scene()
{

}

void Scene::setCameraPos(const float x, const float y, const float z)
{
    cam_.pos_ = {x,y,z};
}

void Scene::setCameraTarget(const float x, const float y, const float z)
{
    cam_.target_ = {x,y,z};
}

void Scene::setCameraNormal(const float x, const float y, const float z)
{
    cam_.up_ = {x,y,z};
}

const Scene::Camera *Scene::getCamera() const
{
    return &cam_;
}

void Scene::update(float elapsedTime)
{
    if (cam_.state_ == Camera::State::MOVING)
    {
        cam_.pos_[0] += cam_.rotationalSpeed_[1]*elapsedTime;
    }
}

void Scene::handleTouchDrag(float x, float y)
{
    if (cam_.state_ == Camera::State::STATIONARY)
    {
        cam_.state_ = Camera::State::MOVING;
    }

    //Logger::logd(TAG_+"Drag event X: "+std::to_string(x)+" Y: "+std::to_string(y));
    cam_.rotationalSpeed_[1] = 80*x;
}

void Scene::handleTouchUp(float x, float y)
{
    if (cam_.state_ == Camera::State::MOVING)
    {
        cam_.state_ = Camera::State::STATIONARY;
        cam_.rotationalSpeed_ = {0,0,0};
    }
}