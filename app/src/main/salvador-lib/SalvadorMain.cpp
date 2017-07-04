//
// Created by jp on 23/06/2017.
//

#include <memory>
#include <chrono>

#include "Renderer/GraphicsAPI.h"
#include "Renderer/GLES3/GLES3Renderer.h"
#include "Logger/Logger.h"
#include "SalvadorMain.h"

SalvadorMain::SalvadorMain() : renderer_(nullptr)
{
}

SalvadorMain::~SalvadorMain()
{
    // TODO: Release all resources
}

SalvadorMain* SalvadorMain::getInstance()
{
    static SalvadorMain instance_;
    return &instance_;
}

void SalvadorMain::initialiseRenderer()
{
    if (!renderer_)
    {
        // Instantiate the appropriate graphics API and inject it into the renderer
        auto api = std::static_pointer_cast<GraphicsAPI>(std::make_shared<GLES3Renderer>());
        renderer_ = std::make_unique<Renderer>(api);
    }
}

std::chrono::duration<float> getElapsedTime()
{
    static auto prevTimeStamp = std::chrono::system_clock::now();

    auto currentTimeStamp = std::chrono::system_clock::now();
    auto elapsedTime = currentTimeStamp-prevTimeStamp;
    prevTimeStamp = currentTimeStamp;

    return std::chrono::duration_cast< std::chrono::duration<float> >(elapsedTime);
}

void SalvadorMain::runTick()
{
    // Update simulation/scene
    scene_.update(getElapsedTime().count());

    renderer_->renderFrame(&scene_);
}

void SalvadorMain::resizeWindow(const int width, const int height)
{
    auto cam = scene_.getCamera();
    renderer_->resizeWindow(width, height, cam->near_, cam->far_, cam->fov_);
}