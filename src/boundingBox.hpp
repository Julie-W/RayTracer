#pragma once
#include <vecmath.h>
#include <vector>
#include "triangle.hpp"
#include "ray.hpp"

class BoundingBox {
private:
    Matrix4f transf;
    std::vector<BoundingBox> children;
    std::vector<Triangle*> triangles;
    float x0, x1, y0, y1, z0, z1;
public:
    BoundingBox(){};
    BoundingBox(float, float, float, float, float, float);
    void setPoints(float, float, float, float, float, float);
    void divideBoundingBox(std::vector<Triangle*>);
    HitPoint shootRay (Ray&);
};
