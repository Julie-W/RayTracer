#include "light.hpp"

Vector3f Light::lightObject(HitPoint &objectHP, Vector3f position){
    Vector3f hitColor = Vector3f(0,0,0);
    Vector3f eyedirn = (-1 *objectHP.point).normalized();
    Vector3f l, h;
    //point
    l = (position - objectHP.point).normalized();
    h = (l + eyedirn).normalized();
    float max = std::max(Vector3f::dot(objectHP.normal,l),(float) 0);
    hitColor = hitColor + color * objectHP.color * max;
        //+ specular * pow(max(dot(objectHP.normal,h),0.0),shininess));
    return hitColor;
}