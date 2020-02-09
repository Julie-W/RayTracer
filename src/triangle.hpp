#pragma once
#include <iostream>
#include <algorithm>
#include "object.hpp"

class Triangle : public Object {
private:
    Vector3f a;
    Vector3f b;
    Vector3f c;
    Vector3f normal;
    Vector3f color = Vector3f(1,1,1);
    Matrix4f transf;
public:
    Triangle(Vector3f, Vector3f, Vector3f);
    Triangle(Vector3f, Vector3f, Vector3f, Vector3f);
    float getMinCoord(int);
    float getMaxCoord(int);
    HitPoint shootRay (Ray&, bool isLight = false);
};