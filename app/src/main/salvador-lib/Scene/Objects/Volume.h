//
// Created by jp on 7/07/2017.
//

#ifndef VOLUMERENDERER_VOLUME_H
#define VOLUMERENDERER_VOLUME_H

#include <vector>

#include "Object.h"

class Volume : public Object
{
    class impl_;
    std::unique_ptr<impl_> pimpl_;

public:
    struct Vertex
    {
        float pos_[3];
        float uv_[2];
    };

    Volume();
    ~Volume() override;

    void loadVolume(std::shared_ptr<std::vector<unsigned char>> volBuffer, unsigned short width,
                              unsigned short height, unsigned short depth);

    // Returns a pointer to an interleaved vertex buffer
    const std::vector<Volume::Vertex>* getGeometry() const;

    // Return the number of vertical cross-sections that need to be rendered
    const int getNumberOfCrossSections() const;

    // Returns the depth of the volume along the current axis
    const float getDepthOnCurrentAxis() const;

    void getDimensions(float& width, float& height, float& depth) const;

    const std::shared_ptr<std::vector<unsigned char>> getTextureData() const;
};

#endif //VOLUMERENDERER_VOLUME_H
