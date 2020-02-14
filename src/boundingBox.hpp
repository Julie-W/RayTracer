#pragma once
#include <vecmath.h>
#include <vector>
#include "triangle.hpp"
#include "ray.hpp"

class BoundingBox {
private:
    Matrix4f transf;
    std::vector<BoundingBox*> children;
    std::vector<Triangle*> triangles;
    Vector3f color = Vector3f(0,0,0);
    float x0, x1, y0, y1, z0, z1;
    HitPoint isHit(Ray&);
    void printBB();
public:
    BoundingBox(){};
    BoundingBox(float, float, float, float, float, float);
    void setPoints(float, float, float, float, float, float);
    void setTriangles(std::vector<Triangle*>);
    void addTriangle(Triangle*);
    void divideBoundingBox(int level);
    bool isIn(Vector3f);
    Vector3f getP0();
    Vector3f getP1();
    HitPoint shootRay (Ray&, bool);
};
