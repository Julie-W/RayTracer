#pragma once
#include <vecmath.h>
#include "ray.hpp"

class Plane {
private:
    Vector3f a;
    Vector3f normal;
    Matrix4f transf;
public:
    Plane (Vector3f,Vector3f,Vector3f);
    void setTransformation (Matrix4f);
    HitPoint shootRay (Ray&);
};