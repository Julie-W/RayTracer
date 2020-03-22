#pragma once
#include "light.hpp"

class PointLight : public Light  {
private:
    Matrix4f transf;
    Vector3f position;
    Vector3f color = Vector3f(1,1,1);
public:
    PointLight (Vector3f);
    PointLight (Vector3f, Vector3f);
    Vector3f lightObject(HitPoint&);
};