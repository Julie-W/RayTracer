#include "triangle.hpp"
static float eps = 0.00001;

Triangle::Triangle(Vector3f p1, Vector3f p2, Vector3f p3) {
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    normal = Vector3f::cross(p1-p2,p1-p3).normalized();
}

Triangle::Triangle(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f col) {
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    normal = Vector3f::cross(p1-p2,p1-p3).normalized();
    color = col;
}

void Triangle::setVertexNormals(Vector3f n1, Vector3f n2, Vector3f n3){
    vNormals.push_back(n1);
    vNormals.push_back(n2);
    vNormals.push_back(n3);
}

HitPoint Triangle::shootRay(Ray &ray, bool isLight){
    HitPoint hitPoint = {};
    float top = Vector3f::dot(vertices[0]-ray.getStart(),normal);
    float bottom = Vector3f::dot(ray.getDirection(),normal);
    if (bottom != 0) {
        float distance = top / bottom;
        if (distance > eps){
            float beta = ((ray.getStart()[0]+distance*ray.getDirection()[0]-vertices[2][0])*(vertices[0][1]-vertices[2][1])-(ray.getStart()[1]+distance*ray.getDirection()[1]-vertices[2][1])*(vertices[0][0]-vertices[2][0]))
                /((vertices[1][0]-vertices[2][0])*(vertices[0][1]-vertices[2][1])-(vertices[1][1]-vertices[2][1])*(vertices[0][0]-vertices[2][0]));
            float alpha;
            if((vertices[0][0]-vertices[2][0] > eps) || (vertices[0][0]-vertices[2][0] < -eps)){
                alpha = (ray.getStart()[0] + ray.getDirection()[0]*distance - vertices[2][0] - (vertices[1][0]-vertices[2][0])*beta)/(vertices[0][0]-vertices[2][0]);
            } else if((vertices[0][1]-vertices[2][1] > eps) || (vertices[0][1]-vertices[2][1] < -eps)){
                alpha = (ray.getStart()[1] + ray.getDirection()[1]*distance - vertices[2][1] - (vertices[1][1]-vertices[2][1])*beta)/(vertices[0][1]-vertices[2][1]);
            } else {
                alpha = (ray.getStart()[2] + ray.getDirection()[2]*distance - vertices[2][2] - (vertices[1][2]-vertices[2][2])*beta)/(vertices[0][2]-vertices[2][2]);
            }
            float gamma = 1 - alpha - beta;
            if (alpha >= -eps && alpha < 1+eps && beta >= -eps && beta < 1+eps && gamma >= -eps && gamma < 1+eps){
                hitPoint.distance = distance;
                hitPoint.isHit = true;
                hitPoint.normal = alpha*vNormals[0] + beta*vNormals[1] + gamma*vNormals[2];
            }
            if (!hitPoint.isHit){
                hitPoint.color = Vector3f(0);
            }
        }
    }
    hitPoint.point = ray.getStart() + hitPoint.distance * ray.getDirection();
    hitPoint.color = color;
    return hitPoint;
}

Vector3f Triangle::getVertex(int i){
    return vertices[i];
}

float Triangle::getMinCoord(int index){
    return std::min(vertices[0][index],std::min(vertices[1][index],vertices[2][index]));
}

float Triangle::getMaxCoord(int index){
    return std::max(vertices[0][index],std::max(vertices[1][index],vertices[2][index]));
}
