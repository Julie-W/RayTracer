#pragma once
#include <vecmath.h>
#include "ray.hpp"
#include "object.hpp"

class Plane : public Object {
private:
    Vector3f a;
    Vector3f normal;
    Matrix4f transf;
    Vector3f color;
public:
    Plane (Vector3f,Vector3f,Vector3f,Vector3f);
    virtual HitPoint shootRay (Ray&);
};