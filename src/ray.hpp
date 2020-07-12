#pragma once
#include <vecmath.h>

struct HitPoint {
    float distance = -1;
    bool isHit = false;
    Vector3f point;
    Vector3f normal;
    Vector3f color = Vector3f(0,0,0);
};

class Ray {
private:
    Vector3f direction;
    Vector3f start;
public:
    Ray (Vector3f, int);
    Ray (Vector3f, Vector3f);
    Vector3f getDirection();
    Vector3f getStart();
};