#ifndef UTILS_H
#define UTILS_H

#include "ofMain.h"

static int nearest(float a, float b)
{
    return (int)floor(a - fmod(a, b));
}

#endif