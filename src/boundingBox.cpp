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
    color = Vector3f(rand() / (RAND_MAX + 1.),rand() / (RAND_MAX + 1.),rand() / (RAND_MAX + 1.));
    color.print();
}

void BoundingBox::setPoints(float numx0, float numx1, float numy0, float numy1, float numz0, float numz1){
    x0 = numx0;
    x1 = numx1;
    y0 = numy0;
    y1 = numy1;
    z0 = numz0;
    z1 = numz1;
}

void BoundingBox::setTriangles(std::vector<Triangle*> vec){
    triangles = vec;
}

void BoundingBox::addTriangle(Triangle* triangle){
    triangles.push_back(triangle);
}

void BoundingBox::divideBoundingBox(int level){
    std::vector<Vector3f> points;
    points.push_back(Vector3f(x0,y0,z0));
    points.push_back(Vector3f((x0+x1)/2+eps,(y0+y1)/2+eps,(z0+z1)/2+eps));
    points.push_back(Vector3f(x1,y1,z1));
    for (int x = 0; x < 2; x++){
        for (int y = 0; y < 2; y++){
            for (int z = 0; z < 2; z++){
                BoundingBox* bb = new BoundingBox(points[x][0],points[x+1][0],points[y][1],points[y+1][1],points[z][2],points[z+1][2]);
                children.push_back(bb);
            }
        }
    }
    //std::cout << triangles.size() << std::endl;
    for (int i = 0; i < triangles.size(); i++){
        for (int j = 0; j < children.size(); j++){
            if (children[j]->isIn(triangles[i]->getVertex(0)) || children[j]->isIn(triangles[i]->getVertex(1)) || children[j]->isIn(triangles[i]->getVertex(2))){
                children[j]->addTriangle(triangles[i]);
            }
        }
    }

    if (level > 0){
        for (int j = 0; j < children.size(); j++){
            if(children[j]->triangles.size() > 0){ 
                children[j]->divideBoundingBox(level-1);
            }
        }
    }
}

bool BoundingBox::isIn(Vector3f point){
    return point[0] >= (x0 - eps) && point[0] <= (x1 + eps)
        && point[1] >= (y0 - eps) && point[1] <= (y1 + eps)
        && point[2] >= (z0 - eps) && point[2] <= (z1 + eps);
}

HitPoint BoundingBox::shootRay(Ray &ray, bool isLight){
    HitPoint hitPoint;
    HitPoint bbHit = isHit(ray);
    if (bbHit.isHit){
        hitPoint = bbHit;
        hitPoint.color = 0.1 * color;
        if (children.size() > 0){
            //hhitPoint = {};
            float distance = 0.0;
            for (int i = 0; i < children.size(); i++){
                HitPoint newHit = children[i]->shootRay(ray, isLight);
                distance = std::max(newHit.distance,distance);
                if(newHit.isHit) hitPoint.color = hitPoint.color + newHit.color;
                /* if(newHit.isHit && (!hitPoint.isHit || (eps < newHit.distance && newHit.distance < hitPoint.distance))){
                    hitPoint = newHit;
                    if(isLight){
                        break;
                    }
                } */
                //hitPoint.distance = distance;
            }
        } /* else {
            hitPoint = {}; */
            /* float distance = 0.0;
            for (int i = 0; i < triangles.size(); i++){
                HitPoint newHit = triangles[i]->shootRay(ray);
                distance = std::max(newHit.distance,distance);
                if(newHit.isHit && (!hitPoint.isHit || newHit.distance < hitPoint.distance)){
                    hitPoint = newHit;
                    if(isLight){
                        break;
                    }
                }
            }
            hitPoint.isHit = true;
            hitPoint.distance = distance; */
/*             hitPoint = bbHit;
            hitPoint.color = color;
        } */
    }
    return hitPoint;
}

HitPoint BoundingBox::isHit(Ray &ray){
    HitPoint hitPoint;
    float tx0, tx1, ty0, ty1, tz0, tz1, t0, t1;
    if (ray.getDirection()[0] == 0){
        tx0 = -INFINITY;
        tx1 = INFINITY;
    } else {
        float a = 1.0 / ray.getDirection()[0];
        if (a >= 0){
            tx0 = (x0 - ray.getStart()[0]) * a;
            tx1 = (x1 - ray.getStart()[0]) * a;
        } else {
            tx1 = (x0 - ray.getStart()[0]) * a;
            tx0 = (x1 - ray.getStart()[0]) * a;
        }
    }
    if (ray.getDirection()[1] == 0){
        ty0 = -INFINITY;
        ty1 = INFINITY;
    } else {
        float b = 1.0 / ray.getDirection()[1];
        if (b >= 0){
            ty0 = (y0 - ray.getStart()[1]) * b;
            ty1 = (y1 - ray.getStart()[1]) * b;
        } else {
            ty1 = (y0 - ray.getStart()[1]) * b;
            ty0 = (y1 - ray.getStart()[1]) * b;
        }
    }
    if (ray.getDirection()[2] == 0){
        tz0 = -INFINITY;
        tz1 = INFINITY;
    } else {
        float c = 1.0 / ray.getDirection()[2];
        if (c >= 0){
            tz0 = (z0 - ray.getStart()[2]) * c;
            tz1 = (z1 - ray.getStart()[2]) * c;
        } else {
            tz1 = (z0 - ray.getStart()[2]) * c;
            tz0 = (z1 - ray.getStart()[2]) * c;
        }
    }

    if (tx0 > ty0) t0 = tx0;
    else t0 = ty0;
    if (tz0 > t0) t0 = tz0;
    if (tx1 < ty1) t1 = tx1;
    else t1 = ty1;
    if (tz1 < t1) t1 = tz1;
    
    if (t0 < t1 && t1 > eps){
        hitPoint.isHit = true;
        if (t0 > eps) hitPoint.distance = t0;
        else hitPoint.distance = t1;
        hitPoint.point = ray.getStart() + hitPoint.distance * ray.getDirection();
    }
    return hitPoint;
}

Vector3f BoundingBox::getP0(){
    return Vector3f(x0,y0,z0);
}

Vector3f BoundingBox::getP1(){
    return Vector3f(x1,y1,z1);
}