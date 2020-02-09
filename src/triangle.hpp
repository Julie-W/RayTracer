#pragma once
#include "object.hpp"

class Triangle : public Object {
private:
    Vector3f a;
    Vector3f b;
    Vector3f c;
    Vector3f normal;
    Vector3f color;
    Matrix4f transf;
public:
    Triangle(Vector3f, Vector3f, Vector3f, Vector3f);
    HitPoint shootRay (Ray&);
};