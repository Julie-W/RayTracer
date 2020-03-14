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
    bool antiAliasing = false;
    Scene* scene;
public:
    ViewPlane(int, int, int, int);
    void setScene(Scene*);
    Vector3f getPixelColor(int, int);
};