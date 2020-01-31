#pragma once
#include <vecmath.h>
#include "ray.hpp"

class Sphere {
private:
    int radius;
    Vector3f middle;
    Matrix4f transf;
public:
    Sphere (int, Vector3f);
    void setTransformation (Matrix4f);
    HitPoint shootRay (Ray&);
};