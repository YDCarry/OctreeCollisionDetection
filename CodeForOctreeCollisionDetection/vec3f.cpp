//
//  vec3f.cpp
//  OctreeCollisionDetection
//
//  Created by Ydy on 16/3/31.
//  Copyright © 2016年 Ydy. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#include "vec3f.h"

using namespace std;

Vec3f::Vec3f()
{
    
}

Vec3f::Vec3f(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

float &Vec3f::operator[](int index)
{
    return v[index];
}

float Vec3f::operator[](int index) const
{
    return v[index];
}

Vec3f Vec3f::operator*(float scale) const
{
    return Vec3f(v[0] * scale, v[1] * scale, v[2] * scale);
}

Vec3f Vec3f::operator/(float scale) const
{
    return Vec3f(v[0] / scale, v[1] / scale, v[2] / scale);
}

Vec3f Vec3f::operator+(const Vec3f &other) const
{
    return Vec3f(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
}

Vec3f Vec3f::operator-(const Vec3f &other) const
{
    return Vec3f(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
}

Vec3f Vec3f::operator-() const
{
    return Vec3f(-v[0], -v[1], -v[2]);
}

const Vec3f &Vec3f::operator*=(float scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
    return *this;
}

const Vec3f &Vec3f::operator/=(float scale)
{
    v[0] /= scale;
    v[1] /= scale;
    v[2] /= scale;
    return *this;
}

const Vec3f &Vec3f::operator+=(const Vec3f &other)
{
    v[0] += other.v[0];
    v[1] += other.v[1];
    v[2] += other.v[2];
    return *this;
}

const Vec3f &Vec3f::operator-=(const Vec3f &other)
{
    v[0] -= other.v[0];
    v[1] -= other.v[1];
    v[2] -= other.v[2];
    return *this;
}

float Vec3f::Magnitude() const
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float Vec3f::MagnitudeSquared() const
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

Vec3f Vec3f::Normalize() const
{
    float m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return Vec3f(v[0] / m, v[1] / m, v[2] / m);
}

float Vec3f::Dot(const Vec3f &other) const
{
    return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}

Vec3f Vec3f::Cross(const Vec3f &other) const
{
    return Vec3f(v[1] * other.v[2] - v[2] * other.v[1],
                 v[2] * other.v[0] - v[0] * other.v[2],
                 v[0] * other.v[1] - v[1] * other.v[0]);
}

Vec3f operator*(float scale, const Vec3f &v)
{
    return v * scale;
}

ostream &operator<<(ostream &output, const Vec3f &v)
{
    cout << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
    return output;
}
