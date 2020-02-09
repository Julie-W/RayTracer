#include "boundingBox.hpp"
#include <iostream>
#include <algorithm>
static float eps = 0.001;

BoundingBox::BoundingBox(float numx0, float numx1, float numy0, float numy1, float numz0, float numz1){
    x0 = numx0;
    x1 = numx1;
    y0 = numy0;
    y1 = numy1;
    z0 = numz0;
    z1 = numz1;
}

void BoundingBox::setPoints(float numx0, float numx1, float numy0, float numy1, float numz0, float numz1){
    x0 = numx0;
    x1 = numx1;
    y0 = numy0;
    y1 = numy1;
    z0 = numz0;
    z1 = numz1;
}

void BoundingBox::divideBoundingBox(std::vector<Triangle*> vec){
    std::vector<Vector3f> points;
    points.push_back(Vector3f(x0,y0,z0));
    points.push_back(Vector3f((x0+x1)/2,(y0+y1)/2,(z0+z1)/2));
    points.push_back(Vector3f(x1,y1,z1));
    for (int x = 0; x < 2; x++){
        for (int y = 0; y < 2; y++){
            for (int z = 0; z < 2; z++){
                children.push_back(BoundingBox(points[x][0],points[x+1][0],points[y][1],points[y+1][1],points[z][2],points[z+1][2]));
            }
        }
    }
}

HitPoint BoundingBox::shootRay(Ray &ray){
    HitPoint hitPoint;
    float tx0, tx1, ty0, ty1, tz0, tz1, t0, t1;
    float a = 1.0 / ray.getDirection()[0];
    if (a >= 0){
        tx0 = (x0 - ray.getStart()[0]) * a;
        tx1 = (x1 - ray.getStart()[0]) * a;
    } else {
        tx1 = (x0 - ray.getStart()[0]) * a;
        tx0 = (x1 - ray.getStart()[0]) * a;
    }
    float b = 1.0 / ray.getDirection()[1];
    if (b >= 0){
        ty0 = (y0 - ray.getStart()[1]) * b;
        ty1 = (y1 - ray.getStart()[1]) * b;
    } else {
        ty1 = (y0 - ray.getStart()[1]) * b;
        ty0 = (y1 - ray.getStart()[1]) * b;
    }
    float c = 1.0 / ray.getDirection()[2];
    if (c >= 0){
        tz0 = (z0 - ray.getStart()[2]) * c;
        tz1 = (z1 - ray.getStart()[2]) * c;
    } else {
        tz1 = (z0 - ray.getStart()[2]) * c;
        tz0 = (z1 - ray.getStart()[2]) * c;
    }

    if (tx0 > ty0) t0 = tx0;
    else t0 = ty0;
    if (tz0 > t0) t0 = tz0;
    if (tx1 < ty1) t1 = tx1;
    else t1 = ty1;
    if (tz1 < t1) t1 = tz1;
    
    if (t0 < t1 && t1 > eps){
        hitPoint.isHit = true;
        if (t0 > eps) {
            hitPoint.distance = t0;
        } else {
            hitPoint.distance = t1;
        }
    }
    return hitPoint;
}