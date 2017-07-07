//
// Created by jp on 7/07/2017.
//

#ifndef VOLUMERENDERER_VOLUME_H
#define VOLUMERENDERER_VOLUME_H

#include <vector>

#include "Object.h"

class Volume : public Object
{
public:

    struct Vertex
    {
        float pos_[3];
        unsigned char col_[4];
        // TODO: Add u,v coordinates
    };

    Volume();
    ~Volume() override;

    // Returns a pointer to an interleaved vertex buffer
    const std::vector<Volume::Vertex>* getGeometry() const;
};

#endif //VOLUMERENDERER_VOLUME_H
