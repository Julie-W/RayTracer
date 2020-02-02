#pragma once
#include <vecmath.h>
#include "ray.hpp"
#include "object.hpp"

class Sphere: public Object {
private:
    int radius;
    Vector3f middle;
    Vector3f color;
    Matrix4f transf;
public:
    Sphere (int, Vector3f, Vector3f);
    virtual HitPoint shootRay (Ray&);
};