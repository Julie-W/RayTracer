#include "scene.hpp"
#include "light.hpp"

Scene::Scene (){
    transf = Matrix4f();
}

void Scene::setTransformation(Matrix4f mat){
    transf = mat;
}

void Scene::addObject(Object *obj){
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
        Object* pobj = objects[i];
        HitPoint newHit = pobj->shootRay(ray);
        if(newHit.isHit && (!hitPoint.isHit || newHit.distance < hitPoint.distance)){
            hitPoint = newHit;
        }
    }
    if (hitPoint.isHit){
        // Move normal to correct direction
        if (Vector3f::dot(hitPoint.normal,ray.getDirection())>0.0001){
            hitPoint.normal = -1*hitPoint.normal;
        }
        color = getLighting(hitPoint);
    }
    return color;
}

Vector3f Scene::getLighting(HitPoint &hitPoint){
    Vector3f color = Vector3f(0);
    color = color + Vector3f(0.01); // ambient lighting
    HitPoint lightHit;
    float distance;
    bool isHit;
    for (int i = 0; i < lights.size(); i++)
    {
        isHit = false;
        Ray lightRay = Ray(hitPoint.point,lights[i].getPosition());
        for (int j = 0; j < objects.size(); j++)
        {
            lightHit = objects[j]->shootRay(lightRay); 
            if (lightHit.isHit && lightHit.distance < (Vector3f(lights[i].getPosition()-hitPoint.point)).abs()){
                isHit = true;
                break;
            }
        }
        if (!isHit) {
            color = color + lights[i].lightObject(hitPoint); 
        }
    }
    return color;
}