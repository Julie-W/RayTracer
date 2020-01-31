#include "ray.hpp"

Ray::Ray(Vector3f dir){
    direction = dir.normalized();
    start = Vector3f(0);
}

Ray::Ray(Vector3f start, Vector3f end){
    direction = (end-start).normalized();
    start = start;
}

Vector3f Ray::getDirection(){
    return direction;
}

Vector3f Ray::getStart(){
    return start;
}