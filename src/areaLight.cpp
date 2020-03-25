#pragma once
#include <iostream>
#include <algorithm>
#include "areaLight.hpp"


AreaLight::AreaLight(Rect *rect) : area(rect) {
    area = rect;
}

Vector3f AreaLight::getPosition(int it) {
    int s = (int) sqrt(iterations);
    int xIt = it % s;
    int yIt = it / s;
    Vector3f a = area->getVertex(1);
    Vector3f b = area->getVertex(2);
    float xRand = rand() / (RAND_MAX + 1.);
    float yRand = rand() / (RAND_MAX + 1.);
    return area->getVertex(0) + (xIt + xRand)/s*a + (yIt + yRand)/s*b;
}

Vector3f AreaLight::getLighting(HitPoint &hp, std::vector<Object*> objs) {
    Vector3f color = Vector3f(0);
    for (int i = 0; i < iterations; i++){
        HitPoint lightHit = {};
        bool isHit = false;
        Vector3f itPoint = getPosition(i);
        Ray lightRay = Ray(hp.point, itPoint);
        for (int j = 0; j < objs.size(); j++)
        {
            if (!objs[j]->isEmissive()) {
                lightHit = objs[j]->shootRay(lightRay, true); 
                // put this in ray intersection 
                if (lightHit.isHit && lightHit.distance < (Vector3f(itPoint-hp.point)).abs()){
                    isHit = true;
                    break;
                }
            }
        }
        if (!isHit) {
            color = color + lightObject(hp, itPoint);
        }
    }
    return color / iterations;
}