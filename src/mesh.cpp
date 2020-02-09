#include "mesh.hpp"

Mesh::Mesh(Vector3f c){
    color = c;
    x0 = INFINITY;
    x1 = -INFINITY;
    y0 = INFINITY;
    y1 = -INFINITY;
    z0 = INFINITY;
    z1 = -INFINITY;
}

void Mesh::addTriangle(Triangle *triangle){
    triangles.push_back(triangle);
    x0 = std::min(x0,triangle->getMinCoord(0));
    y0 = std::min(y0,triangle->getMinCoord(1));
    z0 = std::min(z0,triangle->getMinCoord(2));
    x1 = std::max(x1,triangle->getMaxCoord(0));
    y1 = std::max(y1,triangle->getMaxCoord(1));
    z1 = std::max(z1,triangle->getMaxCoord(2));
}

void Mesh::addBoundingBox(){
    boundingBox.setPoints(x0,x1,y0,y1,z0,z1);
    boundingBox.divideBoundingBox(triangles);
}

HitPoint Mesh::shootRay(Ray &ray, bool isLight){
    HitPoint hitPoint;
    if (boundingBox.shootRay(ray).isHit){
        for (int i = 0; i < triangles.size(); i++){
            HitPoint newHit = triangles[i]->shootRay(ray);
            if(newHit.isHit && (!hitPoint.isHit || newHit.distance < hitPoint.distance)){
                hitPoint = newHit;
                if(isLight){
                    break;
                }
            }
        }
    }
    return hitPoint;
}