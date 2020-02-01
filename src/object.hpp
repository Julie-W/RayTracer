#pragma once
#include <vecmath.h>

class Object {
private:
    Matrix4f transf;
public:
    virtual HitPoint shootRay (Ray&) {HitPoint hp; return hp;}
};