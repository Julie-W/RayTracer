#include <iostream>
#include <algorithm>
#include "pointLight.hpp"


PointLight::PointLight(Vector3f pos){
    position = pos;
}

PointLight::PointLight(Vector3f pos, Vector3f col){
    position = pos;
    color = col;
}

Vector3f PointLight::getLighting(HitPoint &hp, std::vector<Object*> objs){
    Vector3f color = Vector3f(0);
    HitPoint lightHit = {};
    bool isHit = false;
    Vector3f point = getPosition();
    Ray lightRay = Ray(hp.point,point);
    for (int j = 0; j < objs.size(); j++)
    {
        if (!objs[j]->isEmissive()) {
            lightHit = objs[j]->shootRay(lightRay, true); 
            // put this in ray intersection 
            if (lightHit.isHit && lightHit.distance < (Vector3f(point-hp.point)).abs()){
                isHit = true;
                break;
            }
        }
    }
    if (!isHit) {
        color = color + lightObject(hp, point); 
    }
    return color;
}