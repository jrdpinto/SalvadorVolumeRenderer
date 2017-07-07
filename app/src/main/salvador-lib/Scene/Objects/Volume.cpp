//
// Created by jp on 7/07/2017.
//

#include "Volume.h"

// Quad arranged in triangle strip format.
const std::vector<Volume::Vertex> QUAD = {{
    {{-1.0f,  -1.0f, 0.0f}, {0xff, 0x00, 0x00, 0xff}},
    {{ 1.0f,  -1.0f, 0.0f}, {0x00, 0x00, 0xff, 0xff}},
    {{-1.0f,   1.0f, 0.0f}, {0x00, 0x00, 0x00, 0xff}},
    {{ 1.0f,   1.0f, 0.0f}, {0x00, 0xff, 0x00, 0xff}},
}};

Volume::Volume()
{
}

Volume::~Volume()
{

}

const std::vector<Volume::Vertex>* Volume::getGeometry() const
{
    return &QUAD;
}