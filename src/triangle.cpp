#include "triangle.hpp"
static float eps = 0.001;

Triangle::Triangle(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f col) {
    a = p1; b = p2; c = p3;
    normal = Vector3f::cross(p1-p2,p1-p3).normalized();
    color = col;
}

HitPoint Triangle::shootRay(Ray &ray){
    HitPoint hitPoint = {};
    float top = Vector3f::dot(a-ray.getStart(),normal);
    float bottom = Vector3f::dot(ray.getDirection(),normal);
    if (bottom != 0) {
        float distance = top / bottom;
        if (distance > eps){
            float beta = ((ray.getStart()[0]+distance*ray.getDirection()[0]-c[0])*(a[1]-c[1])-(ray.getStart()[1]+distance*ray.getDirection()[1]-c[1])*(a[0]-c[0]))
                /((b[0]-c[0])*(a[1]-c[1])-(b[1]-c[1])*(a[0]-c[0]));
            float alpha;
            if((a[0]-c[0] > eps) || (a[0]-c[0] < -eps)){
                alpha = (ray.getStart()[0] + ray.getDirection()[0]*distance - c[0] - (b[0]-c[0])*beta)/(a[0]-c[0]);
            } else if((a[1]-c[1] > eps) || (a[1]-c[1] < -eps)){
                alpha = (ray.getStart()[1] + ray.getDirection()[1]*distance - c[1] - (b[1]-c[1])*beta)/(a[1]-c[1]);
            } else {
                alpha = (ray.getStart()[2] + ray.getDirection()[2]*distance - c[2] - (b[2]-c[2])*beta)/(a[2]-c[2]);
            }
            float gamma = 1 - alpha - beta;
            if (alpha >= eps && alpha < 1 && beta >= eps && beta < 1 && gamma >= eps && gamma < 1){
                hitPoint.distance = distance;
                hitPoint.isHit = true;
            }
        }
    }
    hitPoint.point = hitPoint.distance * ray.getDirection();
    hitPoint.color = color;
    hitPoint.normal = normal;
    return hitPoint;
}
