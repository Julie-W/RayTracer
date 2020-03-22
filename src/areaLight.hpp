#pragma once
#include "light.hpp"
#include "rectangle.hpp"

class AreaLight : public Light  {
private:
    Matrix4f transf;
    Rect *area;
    Vector3f color = Vector3f(1,1,1);
public:
    AreaLight(Rect *rect);
    Vector3f getPosition();
    Vector3f lightObject(HitPoint&);
};