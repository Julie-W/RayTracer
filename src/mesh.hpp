#pragma once
#include <vector>
#include "object.hpp"
#include "triangle.hpp"
#include "boundingBox.hpp"

class Mesh : public Object {
private:
    std::vector<Triangle*> triangles;
    BoundingBox boundingBox;
    Vector3f color;
    Matrix4f transf;
    float x0, x1, y0, y1, z0, z1;
public:
    Mesh(Vector3f);
    void addTriangle(Triangle*);
    void addBoundingBox();
    HitPoint shootRay(Ray&, bool isLight);
};