#include <algorithm>
#include "sphere.hpp"
static float eps = 0.001;

Sphere::Sphere(int r, Vector3f p, Vector3f col) {
    radius = r;
    middle = p;
    color = col;
}

HitPoint Sphere::shootRay(Ray &ray, bool isLight) {
    HitPoint hitPoint = {};
    float a = Vector3f::dot(ray.getDirection(),ray.getDirection());
    float b = 2 * Vector3f::dot(ray.getStart()-middle,ray.getDirection());
    float c = Vector3f::dot(ray.getStart()-middle,ray.getStart()-middle) - radius*radius;
    float determinant = b*b - 4*a*c;
    if (determinant == 0){
        hitPoint.distance = -b / (2*a);
        hitPoint.isHit = true;
    } else if (determinant > 0) {
        float t1, t2;
        t1 = (- b - sqrt(determinant)) / (2*a);
        t2 = (- b + sqrt(determinant)) / (2*a);
        if (t1 > eps && t2 > eps){
            hitPoint.distance = std::min(t1,t2);
            hitPoint.isHit = true;
        } else if (t1 > eps || t2 > eps){
            hitPoint.distance = std::max(t1,t2);
            hitPoint.isHit = true;
        }
    }
    hitPoint.point = hitPoint.distance * ray.getDirection();
    hitPoint.color = color;
    hitPoint.normal = (hitPoint.point - middle).normalized();
    return hitPoint;
}
