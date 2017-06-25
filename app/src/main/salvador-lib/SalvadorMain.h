//
// Created by jp on 23/06/2017.
//

#ifndef SALVADOR_MAIN_H
#define SALVADOR_MAIN_H

#include "Renderer.h"

class SalvadorMain
{
public:
    static SalvadorMain* getInstance();

    void initialiseRenderer();

    // Updates the current scene and prompts the renderer to render a single frame
    void runTick();

    // TODO: Remove this function. NOTHING outside of main should have access to the renderer!
    const std::shared_ptr<Renderer> getRenderer();

private:
    SalvadorMain();
    ~SalvadorMain();

    std::shared_ptr<Renderer> renderer_;

    // Override the copy constructor and assignment operator to ensure that they are not
    // called or implemented.
    SalvadorMain(SalvadorMain& rhs) = delete;
    SalvadorMain& operator =(SalvadorMain& rhs) = delete;
};

#endif //SALVADOR_MAIN_H