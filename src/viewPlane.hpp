#pragma once
#include <vecmath.h>
#include "scene.hpp"

class ViewPlane {
private:
    int distance;
    int width;
    int height;
    float fovx;
    float fovy;
public:
    ViewPlane(int, int, int, int, int);
    Vector3f getPixelColor(int, int, Scene);
};