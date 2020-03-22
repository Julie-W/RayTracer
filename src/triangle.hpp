#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "object.hpp"

class Triangle : public Object {
private:
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> vNormals;
    Vector3f normal;
    Vector3f color = Vector3f(1,1,1);
    Matrix4f transf;
public:
    Triangle(Vector3f, Vector3f, Vector3f);
    Triangle(Vector3f, Vector3f, Vector3f, Vector3f);
    void setVertexNormals(Vector3f, Vector3f, Vector3f);
    Vector3f getVertex(int);
    float getMaxCoord(int);
    float getMinCoord(int);
    HitPoint shootRay (Ray&, bool isLight = false);
};