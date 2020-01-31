#pragma once
#include "transform.hpp"
#include <iostream>

// Helper rotation function.  Please implement this.  
Matrix3f Transform::rotate(const float degrees, const Vector3f &axis) 
{
    Matrix3f transformMat;
    float c = cos(degrees * pi / 180);
    float s = sin(degrees * pi / 180);
    float x = axis[0], y = axis[1], z = axis[2];
    transformMat = 
    Matrix3f(
    c,0,0,
    0,c,0,
    0,0,c).transposed()
    + Matrix3f(
    x*x, x*y, x*z,
    x*y, y*y, y*z,
    x*z, y*z, z*z).transposed().uniformScaling(1 - c)
    + Matrix3f(
    0,-z,y,
    z,0,-x,
    -y, x, 0).transposed().uniformScaling(s);
    return transformMat;
}

void Transform::left(float degrees, Vector3f &eye, Vector3f &up) 
{
    eye = rotate(degrees,up)*eye; 
}

void Transform::up(float degrees, Vector3f &eye, Vector3f &up) 
{
    Vector3f axis = Vector3f::cross(eye,up).normalized();
    eye = rotate(degrees,axis)*eye;
    up = rotate(degrees,axis)*up;
}

Matrix4f Transform::lookAt(const Vector3f &eye, const Vector3f &center, const Vector3f &up) 
{
    Vector3f u,v,w;
    w = eye.normalized();
    u = Vector3f::cross(up,w).normalized();
    v = Vector3f::cross(w,u).normalized();
    Matrix4f rot = Matrix4f(u[0],v[0],w[0],0,u[1],v[1],w[1],0,u[2],v[2],w[2],0,0,0,0,1);
    Matrix4f trans = Matrix4f(); trans.setRow(3, Vector4f(-eye[0],-eye[1],-eye[2],1));
    return rot*trans;
}

Matrix4f Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    float angle = fovy * pi / 180;
    Matrix4f ret = Matrix4f(
        1/(aspect*tan(angle/2)), 0, 0, 0,
        0, 1/tan(angle/2), 0, 0,
        0, 0, (-zFar - zNear)/(zFar - zNear),-2*zFar*zNear/(zFar - zNear),
        0, 0,-1,0 //-tan(angle/2), 0
    ).transposed();
    return ret;
}

Matrix4f Transform::scale(const float sx, const float sy, const float sz) 
{
    return Matrix4f(
    sx,0,0,0,
    0,sy,0,0,
    0,0,sz,0,
    0,0,0,1).transposed();
}

Matrix4f Transform::translate(const float tx, const float ty, const float tz) 
{
    return Matrix4f(
    1,0,0,tx,
    0,1,0,ty,
    0,0,1,tz,
    0,0,0,1).transposed();
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

Vector3f Transform::upvector(const Vector3f &up, const Vector3f &zvec) 
{
    Vector3f x = Vector3f::cross(up,zvec); 
    Vector3f y = Vector3f::cross(zvec,x);
    Vector3f ret = y.normalized(); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
