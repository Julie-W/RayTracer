#pragma once
#include <vecmath.h>
#include "ray.hpp"

class Object {
private:
    Vector3f color;
    Matrix4f transf;
    bool emissive = false;
public:
    virtual HitPoint shootRay (Ray&, bool isLight = false) {HitPoint hp; return hp;}
    virtual void setColor(Vector3f col) {color = col;}
    bool isEmissive() {return emissive;}
    void setEmissive() {emissive = true;}
};