//
// Created by jp on 24/06/2017.
//

#include "Renderer.h"

class Renderer::impl
{
private:

public:
    std::shared_ptr<GraphicsAPI> api_;

    impl(std::shared_ptr<GraphicsAPI> api)
    {
        api_ = api;
        api->clearColour(1.0f, 1.0f, 1.0f, 1.0f);
    }

    ~impl()
    {
    }

};

Renderer::Renderer(std::shared_ptr<GraphicsAPI> api)
{
    pimpl_ = std::make_unique<impl>(api);
}

Renderer::~Renderer()
{
}

void Renderer::resizeWindow(const int width, const int height, const float near, const float far, const float fov)
{
    pimpl_->api_->resizeWindow(width, height, near, far, fov);
}

void Renderer::renderFrame(const Scene* scene)
{
    pimpl_->api_->renderFrame(scene);
}
