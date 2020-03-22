#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "triangle.hpp"

class Rect : public Object {
private:
    std::vector<Triangle> triangles;
    Vector3f normal;
    Vector3f color = Vector3f(1,1,1);
    Matrix4f transf;
public:
    Rect (Vector3f, Vector3f, Vector3f, Vector3f);
    Rect (Vector3f, Vector3f, Vector3f, Vector3f, Vector3f);
    Vector3f getVertex(int);
    HitPoint shootRay (Ray&, bool isLight = false);
};