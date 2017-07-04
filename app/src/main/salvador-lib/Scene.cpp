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

void Scene::setCameraDirection(const float x, const float y, const float z)
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