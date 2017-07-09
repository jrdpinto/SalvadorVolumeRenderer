//
// Created by jp on 7/07/2017.
//

#include "Volume.h"
#include "../../Logger/Logger.h"

const std::string TAG_ = "Volume.cpp";

// Quad arranged in triangle strip format.
const std::vector<Volume::Vertex> QUAD = {{
    {{-1.0f,  -1.0f, 0.0f}, {0xff, 0x00, 0x00, 0xff}},
    {{ 1.0f,  -1.0f, 0.0f}, {0x00, 0x00, 0xff, 0xff}},
    {{-1.0f,   1.0f, 0.0f}, {0x00, 0x00, 0x00, 0xff}},
    {{ 1.0f,   1.0f, 0.0f}, {0x00, 0xff, 0x00, 0xff}},
}};

Volume::Volume()
{
    volBuffer_ = nullptr;
}

Volume::~Volume()
{

}

const std::vector<Volume::Vertex>* Volume::getGeometry() const
{
    return &QUAD;
}

void initGeometry(unsigned short width, unsigned short height, unsigned short depth)
{

}

void Volume::loadVolume(std::shared_ptr<std::vector<float>> volBuffer,
                        unsigned short width, unsigned short height, unsigned short depth)
{
    if (!volBuffer->empty())
    {
        volBuffer_ = volBuffer;
        width_ = width;
        height_ = height;
        depth_ = depth;
    }
}
