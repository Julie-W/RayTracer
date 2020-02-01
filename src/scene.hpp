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
    std::vector<Sphere> objects;
    std::vector<Light> lights;
    Vector3f getLighting(HitPoint&);
public:
    Scene ();
    void setTransformation (Matrix4f);
    void addObject(Sphere&);
    void addLight(Light&);
    Vector3f getColor(Ray&);
};