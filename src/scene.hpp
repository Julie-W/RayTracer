#pragma once
#include <vector>
#include <vecmath.h>
#include <iostream>
#include "sphere.hpp"
#include "plane.hpp"
#include "light.hpp"
#include "ray.hpp"

class Scene {
private:
    Matrix4f transf;
    std::vector<Object*> objects;
    std::vector<Light> lights;
    Vector3f getLighting(HitPoint&);
    Vector3f lightObject(HitPoint&);
public:
    Scene ();
    void setTransformation (Matrix4f);
    void addObject(Object*);
    void addLight(Light&);
    Vector3f getColor(Ray&);
};