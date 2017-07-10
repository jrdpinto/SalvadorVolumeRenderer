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

class Volume::impl_
{
public:
    // Dimensions
    unsigned short width_, height_, depth_;

    // The number of vertical cross-sections, used to sample the volume at regular intervals.
    int numOfCrossSections_;

    // Typical volume datasets are quite large. 'scale_' is used to shrink them down to a more
    // manageable size.
    float scale_;

    // Holds the raw data loaded from the dataset.
    std::shared_ptr<std::vector<unsigned char>> volBuffer_;

    // The quad used to render a cross-section of the volume.
    std::vector<Volume::Vertex> geometry_;

    impl_() : width_(0), height_(0), depth_(0), scale_(0.1f), numOfCrossSections_(0.0f),
              volBuffer_(nullptr)
    {
    }

    ~impl_()
    {
    }

    void initGeometry()
    {
        // X-Y plane (front facing)
        // Quad is drawn with its centre at the origin
        float scaledWidth = (width_*scale_)/2.0f;
        float scaledHeight = (height_*scale_)/2.0f;
        geometry_ = {{
                {{-scaledWidth, -scaledHeight, 0.0f}, {0xff, 0x00, 0x00, 0xff}},
                {{ scaledWidth, -scaledHeight, 0.0f}, {0x00, 0x00, 0xff, 0xff}},
                {{-scaledWidth,  scaledHeight, 0.0f}, {0x00, 0x00, 0x00, 0xff}},
                {{ scaledWidth,  scaledHeight, 0.0f}, {0x00, 0xff, 0x00, 0xff}},
        }};
        numOfCrossSections_ = depth_;
    }

    void loadVolume(std::shared_ptr<std::vector<unsigned char>> volBuffer, unsigned short width,
                              unsigned short height, unsigned short depth)
    {
        if (!volBuffer->empty())
        {
            volBuffer_ = volBuffer;
            width_ = width;
            height_ = height;
            depth_ = depth;

            initGeometry();
        }
    }

    // Returns depth along the current axis
    float getDepthOnCurrentAxis()
    {
        return depth_*scale_;
    }

    void getDimensions(float& width, float& height, float& depth)
    {
        width = width_;
        height = height_;
        depth = depth_;
    }
};


Volume::Volume()
{
    pimpl_ = std::make_unique<impl_>();
}

Volume::~Volume()
{

}

const std::vector<Volume::Vertex>* Volume::getGeometry() const
{
    return &pimpl_->geometry_;
}

void Volume::loadVolume(std::shared_ptr<std::vector<unsigned char>> volBuffer,
                        unsigned short width, unsigned short height, unsigned short depth)
{
    pimpl_->loadVolume(volBuffer, width, height, depth);
}

const int Volume::getNumberOfCrossSections() const
{
    return pimpl_->numOfCrossSections_;
}

const float Volume::getDepthOnCurrentAxis() const
{
    return pimpl_->getDepthOnCurrentAxis();
}

void Volume::getDimensions(float &width, float &height, float &depth) const
{
    pimpl_->getDimensions(width, height, depth);
}

const std::shared_ptr<std::vector<unsigned char>> Volume::getTextureData() const
{
    return pimpl_->volBuffer_;
}
