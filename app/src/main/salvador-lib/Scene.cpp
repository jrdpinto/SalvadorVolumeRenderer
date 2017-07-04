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
    cam_.pos_[0] = x;
    cam_.pos_[1] = y;
    cam_.pos_[2] = z;
}

void Scene::setCameraDirection(const float x, const float y, const float z)
{
    cam_.target_[0] = x;
    cam_.target_[1] = y;
    cam_.target_[2] = z;
}

void Scene::setCameraNormal(const float x, const float y, const float z)
{
    cam_.up_[0] = x;
    cam_.up_[1] = y;
    cam_.up_[2] = z;
}

const Scene::Camera *Scene::getCamera() const
{
    return &cam_;
}