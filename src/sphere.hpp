#pragma once
#include <vecmath.h>
#include "ray.hpp"
#include "object.hpp"

class Sphere: public Object {
private:
    int radius;
    Vector3f middle;
    Matrix4f transf;
public:
    Sphere (int, Vector3f);
    virtual HitPoint shootRay (Ray&);
};