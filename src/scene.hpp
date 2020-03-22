#pragma once
#include <vector>
#include <vecmath.h>
#include <iostream>
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "rectangle.hpp"
#include "mesh.hpp"
#include "ray.hpp"
#include "areaLight.hpp"
#include "pointLight.hpp"


class Scene {
private:
    Matrix4f transf;
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Vector3f ambient;
    Vector3f getLighting(HitPoint&);
    Vector3f lightObject(HitPoint&);
public:
    Scene ();
    void setTransformation (Matrix4f);
    void addObject(Object*);
    void addLight(Light*);
    void addAmbient(Vector3f);
    Vector3f getColor(Ray&);
};