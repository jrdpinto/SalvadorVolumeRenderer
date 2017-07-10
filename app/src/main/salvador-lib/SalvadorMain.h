//
// Created by jp on 23/06/2017.
//

#ifndef SALVADOR_MAIN_H
#define SALVADOR_MAIN_H

#include "Renderer/Renderer.h"
#include "Scene/Scene.h"

class SalvadorMain
{
public:
    static SalvadorMain* getInstance();

    void initialiseRenderer();

    // Updates the current scene and prompts the renderer to render a single frame
    void runTick();

    // Resize window and update GUI/scene elements if required
    void resizeWindow(const int width, const int height);

    // Handle touch event that moves across the screen
    void handleDragEvent(float x, float y);

    void handleTouchUp(float x, float y);

    void loadVolume(std::shared_ptr<std::vector<unsigned char>> volBuffer, unsigned short width,
                    unsigned short height, unsigned short depth);

private:
    SalvadorMain();
    ~SalvadorMain();

    std::unique_ptr<Renderer> renderer_;
    Scene scene_;

    // Override the copy constructor and assignment operator to ensure that they are not
    // called or implemented.
    SalvadorMain(SalvadorMain& rhs) = delete;
    SalvadorMain& operator =(SalvadorMain& rhs) = delete;
};

#endif //SALVADOR_MAIN_H