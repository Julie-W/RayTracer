#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "triangle.hpp"

class Rect : public Object {
private:
    std::vector<Vector3f> vertices;
    Vector3f normal;
    Vector3f color = Vector3f(1,1,1);
    bool emissive;
    Matrix4f transf;
public:
    Rect (Vector3f, Vector3f, Vector3f);
    Rect (Vector3f, Vector3f, Vector3f, Vector3f);
    Vector3f getVertex(int);
    HitPoint shootRay (Ray&, bool isLight = false);
};