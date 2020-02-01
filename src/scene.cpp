#include "scene.hpp"

Scene::Scene (){
    transf = Matrix4f();
}

void Scene::setTransformation(Matrix4f mat){
    transf = mat;
}

void Scene::addObject(Sphere &obj){
    objects.push_back(obj);
}

void Scene::addLight(Light &light){
    lights.push_back(light);
}

Vector3f Scene::getColor(Ray &ray){
    Vector3f color;
    HitPoint hitPoint;
    for (int i = 0; i < objects.size(); i++)
    {
        HitPoint newHit = objects[i].shootRay(ray);
        if ((newHit.distance < hitPoint.distance) || hitPoint.distance < 0){
            hitPoint = newHit;
        }
    }
    if (hitPoint.distance > 0){
        std::cout << "Here" << std::endl;
        color = getLighting(hitPoint);
    }
    return color;
}

Vector3f Scene::getLighting(HitPoint &hitPoint){
    Vector3f color = Vector3f(0);
    float distance;
    bool isHit;
    for (int i = 0; i < lights.size(); i++)
    {
        isHit = false;
        Ray lightRay = Ray(hitPoint.point,lights[i].getPosition());
        for (int j = 0; j < objects.size(); j++)
        {
            distance = objects[j].shootRay(lightRay).distance;
            if (distance > 0){
                isHit = true;
                break;
            }
        }
        if (!isHit) {
            color = color + Vector3f(255,0,0);
        }
    }
    return color;
}