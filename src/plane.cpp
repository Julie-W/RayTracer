#include "plane.hpp"
static float eps = 0.001;

Plane::Plane(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f col) {
    a = p1;
    normal = Vector3f::cross(p1-p2,p1-p3).normalized();
    color = col;
}

HitPoint Plane::shootRay(Ray &ray){
    HitPoint hitPoint = {};
    float top = Vector3f::dot(a-ray.getStart(),normal);
    float bottom = Vector3f::dot(ray.getDirection(),normal);
    if (bottom > 0) {
        float distance = top / bottom;
        if (distance > eps){
            hitPoint.distance = distance;
            hitPoint.isHit = true;
        }
    }
    hitPoint.point = hitPoint.distance * ray.getDirection();
    hitPoint.color = color;
    hitPoint.normal = normal;
    return hitPoint;
}
