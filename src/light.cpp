#include "light.hpp"


Light::Light(Vector3f pos){
    position = pos;
}

void Light::setTransformation(Matrix4f mat){
    transf = mat;
}

Vector3f Light::getPosition(){
    return position;
}