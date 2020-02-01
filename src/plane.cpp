#include "plane.hpp"
static float eps = 0.001;

Plane::Plane(Vector3f p1, Vector3f p2, Vector3f p3) {
    a = p1;
    normal = Vector3f::cross(p1-p2,p1-p3).normalized();
}

void Plane::setTransformation(Matrix4f mat){
    transf = mat;
}

HitPoint Plane::shootRay(Ray &ray){
    HitPoint hitPoint = {};
    hitPoint.distance = -1;
    float top = Vector3f::dot(ray.getStart()-a,normal);
    float bottom = Vector3f::dot(ray.getDirection(),normal);
    if (bottom > 0) {
        float distance = top / bottom;
        if (distance > eps){
            hitPoint.distance = distance;
        }
    }
    hitPoint.point = hitPoint.distance * ray.getDirection();
    return hitPoint;
}
