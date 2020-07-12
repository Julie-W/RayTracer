#include "viewPlane.hpp"
#include "ray.hpp"
const float pi = 3.14159265;

ViewPlane::ViewPlane(int d, int w, int h, int xangle){
    distance = d;
    width = w;
    height = h;
    fovx = xangle*pi/180;
    fovy = xangle * pi * ((float)h / (float)w) / 180;
}

void ViewPlane::setScene(Scene* sc){
    scene = sc;
}

Vector3f ViewPlane::getPixelColor(int x, int y){
    Vector3f direction;
    float xCoord = distance * (float)tan(fovx/2)*((x-width/2.0)/(width/2.0));
    float yCoord = distance * (float)tan(fovy/2)*((height/2.0-y)/(height/2.0));
    direction = Vector3f(xCoord,yCoord,-distance);
    Ray ray = Ray(direction, distance);
    return scene->getColor(ray);
}