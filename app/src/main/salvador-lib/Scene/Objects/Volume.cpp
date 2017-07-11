//
// Created by jp on 7/07/2017.
//

#include "Volume.h"
#include "../../Logger/Logger.h"

const std::string TAG_ = "Volume.cpp";

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

    // The quads used to render cross-sections of the volume. Each quad aligns with a particular
    // 2d plane and is drawn by the GPU when the camera is facing it.
    std::vector<Volume::Vertex> yzGeometry_, xzGeometry_, xyGeometry_;

    impl_() : width_(0), height_(0), depth_(0), scale_(0.1f), numOfCrossSections_(0.0f),
              volBuffer_(nullptr)
    {
    }

    ~impl_()
    {
    }

    void initGeometry()
    {
        // Quad is drawn with its centre at the origin
        float scaledWidth = (width_*scale_)/2.0f;
        float scaledHeight = (height_*scale_)/2.0f;
        float scaledDepth = (depth_*scale_)/2.0f;
        xyGeometry_ = {{
                {{-scaledWidth, -scaledHeight, 0.0f}, {0.0f, 0.0f, 0.0f}},
                {{ scaledWidth, -scaledHeight, 0.0f}, {1.0f, 0.0f, 0.0f}},
                {{-scaledWidth,  scaledHeight, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{ scaledWidth,  scaledHeight, 0.0f}, {1.0f, 1.0f, 0.0f}},
        }};
        xzGeometry_ = {{
                {{-scaledWidth, 0.0f,  scaledDepth}, {0.0f, 0.5f, 0.0f}},
                {{ scaledWidth, 0.0f,  scaledDepth}, {1.0f, 0.5f, 0.0f}},
                {{-scaledWidth, 0.0f, -scaledDepth}, {0.0f, 0.5f, 1.0f}},
                {{ scaledWidth, 0.0f, -scaledDepth}, {1.0f, 0.5f, 1.0f}},
        }};
        yzGeometry_ = {{
                {{0.0f, -scaledHeight,  scaledDepth}, {0.5f, 0.0f, 0.0f}},
                {{0.0f, -scaledHeight, -scaledDepth}, {0.5f, 0.0f, 1.0f}},
                {{0.0f,  scaledHeight,  scaledDepth}, {0.5f, 1.0f, 0.0f}},
                {{0.0f,  scaledHeight, -scaledDepth}, {0.5f, 1.0f, 1.0f}},
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

const std::vector<Volume::Vertex>* Volume::getXYGeometry() const
{
    return &pimpl_->xyGeometry_;
}

const std::vector<Volume::Vertex> *Volume::getXZGeometry() const
{
    return &pimpl_->xzGeometry_;
}

const std::vector<Volume::Vertex> *Volume::getYZGeometry() const
{
    return &pimpl_->yzGeometry_;
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