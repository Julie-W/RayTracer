#include "viewPlane.hpp"
#include "ray.hpp"
#include <cstdlib>
#include <ctime>
const float pi = 3.14159265;

ViewPlane::ViewPlane(int d, int w, int h, int xangle){
    distance = d;
    width = w;
    height = h;
    fovx = xangle*pi/180;
    fovy = xangle * pi * ((float)h / (float)w) / 180;
    srand (static_cast <unsigned> (time(0)));
}

void ViewPlane::setScene(Scene* sc){
    scene = sc;
}

Vector3f ViewPlane::getPixelColor(int x, int y){
    Vector3f direction;
    float xCoord = distance * (float)tan(fovx/2)*((x-width/2.0)/(width/2.0));
    float yCoord = distance * (float)tan(fovy/2)*((height/2.0-y)/(height/2.0));
    direction = Vector3f(xCoord,yCoord,-distance);
    Ray ray = Ray(direction);
    if (antiAliasing){
        float xAA = x - 1 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2);
        float yAA = y - 1 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2);
        xCoord = distance * (float)tan(fovx/2)*((xAA-width/2.0)/(width/2.0));
        yCoord = distance * (float)tan(fovy/2)*((height/2.0-yAA)/(height/2.0));
        direction = Vector3f(xCoord,yCoord,-distance);
        Ray rayAA = Ray(direction);
        return (scene->getColor(ray) + scene->getColor(rayAA))/2;
    } else {
        return scene->getColor(ray);
    }
}