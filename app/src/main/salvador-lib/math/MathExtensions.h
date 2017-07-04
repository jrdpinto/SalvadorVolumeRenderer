//
// Created by jp on 4/07/2017.
// Description: A list of helper functions to extend the functionality of 'linmath.h'.
//

#ifndef SALVADOR_MATHEXTENSIONS_H
#define SALVADOR_MATHEXTENSIONS_H

#include <math.h>

static inline float deg_to_rad(const float deg)
{
    return deg*((float) M_PI/180.0f);
}

#endif //SALVADOR_MATHEXTENSIONS_H
