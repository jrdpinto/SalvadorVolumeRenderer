//
// Created by jp on 23/06/2017.
//

#include <memory>

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

void SalvadorMain::runTick()
{
    // TODO: Compute time since last frame

    // TODO: Update simulation/scene

    renderer_->renderFrame(&scene_);
}

void SalvadorMain::resizeWindow(const int width, const int height)
{
    renderer_->resizeWindow(width, height);
}
