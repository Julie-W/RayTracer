#pragma once
#include "light.hpp"

class AreaLight : public Light  {
private:
    Matrix4f transf;
    Rect *area;
    int iterations = 4;
    Vector3f color = Vector3f(1,1,1);
public:
    AreaLight(Rect *rect);
    Vector3f getPosition(int);
    Vector3f getLighting(HitPoint&,std::vector<Object*>);
};