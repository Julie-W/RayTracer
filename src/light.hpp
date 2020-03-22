#pragma once
#include <vecmath.h>
#include "ray.hpp"

class Light {
private:
    Matrix4f transf;
    Vector3f position;
    Vector3f color = Vector3f(1,1,1);
public:
    virtual Vector3f getPosition() {return position;};
    virtual Vector3f lightObject(HitPoint&) {return Vector3f(0);};
};