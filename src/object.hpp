#pragma once
#include <vecmath.h>

class Object {
private:
    Vector3f color;
    Matrix4f transf;
public:
    virtual HitPoint shootRay (Ray&) {HitPoint hp; return hp;}
    virtual void setColor(Vector3f col) {color = col;}
};