#include "rectangle.hpp"
static float eps = 0.00001;

Rect::Rect(Vector3f p1, Vector3f a, Vector3f b) {
    vertices.push_back(p1);
    vertices.push_back(a);
    vertices.push_back(b);
    normal = Vector3f::cross(a,b);
}

Rect::Rect(Vector3f p1, Vector3f a, Vector3f b, Vector3f col) {
    vertices.push_back(p1);
    vertices.push_back(a);
    vertices.push_back(b);
    normal = Vector3f::cross(a,b);
    color = col;
}

Vector3f Rect::getVertex(int index){
    return vertices[index];
}

HitPoint Rect::shootRay(Ray &ray, bool isLight){
    HitPoint hitPoint = {};
    float top = Vector3f::dot(vertices[0]-ray.getStart(),normal);
    float bottom = Vector3f::dot(ray.getDirection(),normal);
    if (bottom != 0) {
        float distance = top / bottom;
        if (distance > eps){
            Vector3f p = ray.getStart() + distance*ray.getDirection();
            float dotA = Vector3f::dot(p - vertices[0],vertices[1]);
            float dotB = Vector3f::dot(p - vertices[0],vertices[2]);
            if ((dotA >= 0) && (dotA <= Vector3f::dot(vertices[1],vertices[1])) 
                && (dotB >= 0) && (dotB <= Vector3f::dot(vertices[2],vertices[2]))) {
                    hitPoint.isHit = true;
                    hitPoint.color = color;
                    hitPoint.distance = distance;
                    hitPoint.normal = normal;
                    hitPoint.point = p;
                    hitPoint.emissive = emissive;
            }
        }
    }
    return hitPoint;
}
