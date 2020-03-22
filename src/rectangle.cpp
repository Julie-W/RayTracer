#include "rectangle.hpp"
static float eps = 0.00001;

Rect::Rect(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4) {
    triangles.push_back(Triangle(p1,p2,p3));
    triangles.push_back(Triangle(p2,p3,p4));
}

Rect::Rect(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4, Vector3f col) {
    triangles.push_back(Triangle(p1,p2,p3,col));
    triangles.push_back(Triangle(p2,p3,p4,col));
    color = col;
}

Vector3f Rect::getVertex(int index){
    if (index == 3){
        return triangles[1].getVertex(2);
    } else {
        return triangles[0].getVertex(index);
    }
}

HitPoint Rect::shootRay(Ray &ray, bool isLight){
    HitPoint hitPoint1 = triangles[0].shootRay(ray, isLight);
    if(hitPoint1.isHit && isLight){
        return hitPoint1;
    } else {
        HitPoint hitPoint2 = triangles[1].shootRay(ray, isLight);
        if (hitPoint1.isHit && (!hitPoint2.isHit || (hitPoint1.distance < hitPoint2.distance))) {
            return hitPoint1;
        } else {
            return hitPoint2;
        }
    }
}
