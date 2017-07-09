//
// Created by jp on 7/07/2017.
//

#include "Volume.h"
#include "../../Logger/Logger.h"

const std::string TAG_ = "Volume.cpp";

// Quad arranged in triangle strip format.
const std::vector<Volume::Vertex> TRIANGLE_STRIP_QUAD = {{
    {{-1.0f,  -1.0f, 0.0f}, {0xff, 0x00, 0x00, 0xff}},
    {{ 1.0f,  -1.0f, 0.0f}, {0x00, 0x00, 0xff, 0xff}},
    {{-1.0f,   1.0f, 0.0f}, {0x00, 0x00, 0x00, 0xff}},
    {{ 1.0f,   1.0f, 0.0f}, {0x00, 0xff, 0x00, 0xff}},
}};

const std::vector<Volume::Vertex> TRIANGLE_QUAD = {{
    {{-1.0f,  -1.0f, -1.0f}, {0xff, 0x00, 0x00, 0xff}},
    {{ 1.0f,  -1.0f, -1.0f}, {0x00, 0x00, 0xff, 0xff}},
    {{-1.0f,   1.0f, -1.0f}, {0x00, 0x00, 0x00, 0xff}},
    {{-1.0f,   1.0f, -1.0f}, {0x00, 0x00, 0x00, 0xff}},
    {{ 1.0f,   1.0f, -1.0f}, {0x00, 0xff, 0x00, 0xff}},
    {{ 1.0f,  -1.0f, -1.0f}, {0x00, 0x00, 0xff, 0xff}},
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
    return &TRIANGLE_STRIP_QUAD;
    //return &TRIANGLE_QUAD;
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
