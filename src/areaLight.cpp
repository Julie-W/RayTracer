#pragma once
#include <iostream>
#include <algorithm>
#include "areaLight.hpp"


AreaLight::AreaLight(Rect *rect) : area(rect) {
    area = rect;
}

Vector3f AreaLight::getPosition() {
    return area->getVertex(0);
}

Vector3f AreaLight::lightObject(HitPoint &objectHP){
    Vector3f hitColor = Vector3f(0,0,0);
    Vector3f eyedirn = (-1 *objectHP.point).normalized();
    Vector3f l, h;
    //point
    l = (getPosition() - objectHP.point).normalized();
    h = (l + eyedirn).normalized();
    float max = std::max(Vector3f::dot(objectHP.normal,l),(float) 0);
    hitColor = hitColor + color * objectHP.color * max;
        //+ specular * pow(max(dot(objectHP.normal,h),0.0),shininess));

    return hitColor;
}