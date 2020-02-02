#pragma once
#include <vecmath.h>
#include "ray.hpp"

class Light {
private:
    Matrix4f transf;
    Vector3f position;
    Vector3f color = Vector3f(1,1,1);
public:
    Light (Vector3f);
    void setTransformation (Matrix4f);
    Vector3f getPosition();
    Vector3f lightObject(HitPoint&);
};