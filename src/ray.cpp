#include "ray.hpp"
#include <algorithm>

Ray::Ray(Vector3f dir){
    direction = dir.normalized();
    start = Vector3f(0);
}

Ray::Ray(Vector3f v1, Vector3f v2){
    direction = (v2-v1).normalized();
    start = v1 + direction*0.5;
}

Vector3f Ray::getDirection(){
    return direction;
}

Vector3f Ray::getStart(){
    return start;
}