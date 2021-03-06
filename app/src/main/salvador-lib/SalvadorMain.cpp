//
// Created by jp on 23/06/2017.
//

#include <memory>
#include <chrono>

#include "Renderer/GraphicsAPI.h"
#include "Renderer/GLES3/GLES3Renderer.h"
#include "Logger/Logger.h"
#include "SalvadorMain.h"
#include "external/linmath/linmath.h"

// Drag vectors with a magnitude lower than this threshold are ignored
static float TOUCH_EVENT_DRAG_THRESHOLD = 0.005f;

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
    float near, far, fov;
    scene_.getCamera()->getProjectionParams(near, far, fov);
    renderer_->resizeWindow(width, height, near, far, fov);
}

void SalvadorMain::handleDragEvent(float x, float y)
{
    auto cancelSmallMovements =
            [](float component)->float
            {
                return abs(component) < TOUCH_EVENT_DRAG_THRESHOLD ? 0.0f : component;
            };
    scene_.handleDragEvent(cancelSmallMovements(x), cancelSmallMovements(y));
}

void SalvadorMain::handleTouchUp(float x, float y)
{
    scene_.handleTouchUp(x,y);
}

void SalvadorMain::loadVolume(std::shared_ptr<std::vector<unsigned char>> volBuffer,
                              unsigned short width,
                              unsigned short height, unsigned short depth)
{
    scene_.loadVolume(volBuffer, width, height, depth);
}