//
// Created by jp on 3/07/2017.
//

#include "Scene.h"

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
    // TODO: Replace this action with movement based on touch input
    cam_.pos_[2] += 0.9f*elapsedTime;
}
