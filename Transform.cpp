// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
#include <iostream>

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 transformMat;
    float c = cos(degrees * pi / 180);
    float s = sin(degrees * pi / 180);
    float x = axis[0], y = axis[1], z = axis[2];
    transformMat = 
    transpose(mat3(
    c,0,0,
    0,c,0,
    0,0,c)) 
    + (1 - c)*transpose(mat3(
    x*x, x*y, x*z,
    x*y, y*y, y*z,
    x*z, y*z, z*z)) 
    + s*transpose(mat3(
    0,-z,y,
    z,0,-x,
    -y, x, 0));
    return transformMat;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    eye = rotate(degrees,up)*eye; 
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    vec3 axis = normalize(cross(eye,up));
    eye = rotate(degrees,axis)*eye;
    up = rotate(degrees,axis)*up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    vec3 u,v,w;
    w = normalize(eye);
    u = normalize(cross(up,w));
    v = normalize(cross(w,u));
    mat4 rot = mat4(u[0],v[0],w[0],0,u[1],v[1],w[1],0,u[2],v[2],w[2],0,0,0,0,1);
    mat4 trans = mat4(); trans[3] = vec4(-eye[0],-eye[1],-eye[2],1);
    return rot*trans;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    float angle = fovy * pi / 180;
    mat4 ret = transpose(mat4(
        1/(aspect*tan(angle/2)), 0, 0, 0,
        0, 1/tan(angle/2), 0, 0,
        0, 0, (-zFar - zNear)/(zFar - zNear),-2*zFar*zNear/(zFar - zNear),
        0, 0,-1,0 //-tan(angle/2), 0
    ));
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    return transpose(mat4(
    sx,0,0,0,
    0,sy,0,0,
    0,0,sz,0,
    0,0,0,1));
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    return transpose(mat4(
    1,0,0,tx,
    0,1,0,ty,
    0,0,1,tz,
    0,0,0,1));
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x);
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
