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

void Renderer::resizeWindow(const int width, const int height)
{
    pimpl_->api_->resizeWindow(width, height);
}

void Renderer::renderFrame()
{
    pimpl_->api_->renderFrame();
}
