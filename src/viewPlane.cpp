#include "viewPlane.hpp"
#include "ray.hpp"
const float pi = 3.14159265;

ViewPlane::ViewPlane(int d, int w, int h, int xangle, int yangle){
    distance = d;
    width = w;
    height = h;
    fovx = xangle*pi/180;
    fovy = yangle*pi/180;
}

Vector3f ViewPlane::getPixelColor(int x, int y, Scene scene){
    Vector3f direction;
    float xCoord = distance * (float)tan(fovx/2)*((x-width/2.0)/(width/2.0));
    float yCoord = distance * (float)tan(fovy/2)*((height/2.0-y)/(height/2.0));
    direction = Vector3f(xCoord,yCoord,-distance);
    Ray ray = Ray(direction);  
    return scene.getColor(ray);
}