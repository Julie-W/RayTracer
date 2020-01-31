#pragma once
#include <vecmath.h>

class Light {
private:
    Matrix4f transf;
    Vector3f position;
public:
    Light (Vector3f);
    void setTransformation (Matrix4f);
    Vector3f getPosition();
};