#pragma once
#include <vecmath.h>

struct HitPoint {
    float distance = -1;
    Vector3f point;
};

class Ray {
private:
    Vector3f direction;
    Vector3f start;
public:
    Ray (Vector3f);
    Ray (Vector3f, Vector3f);
    Vector3f getDirection();
    Vector3f getStart();
};