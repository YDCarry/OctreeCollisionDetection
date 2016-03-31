//
//  vec3f.h
//  OctreeCollisionDetection
//
//  Created by Ydy on 16/3/30.
//  Copyright © 2016年 Ydy. All rights reserved.
//

#ifndef vec3f_h
#define vec3f_h

#include <iostream>

class Vec3f
{
    private:
        float v[3];
    
    public:
    Vec3f();
    Vec3f(float x, float y, float z);
    
    float &operator [] (int index);
    float operator [] (int index) const;
    
    Vec3f operator * (float scale) const;
    Vec3f operator / (float scale) const;
    Vec3f operator + (const Vec3f &other) const;
    Vec3f operator - (const Vec3f &other) const;
    Vec3f operator - () const;
    
    const Vec3f &operator *= (float scale);
    const Vec3f &operator /= (float scale);
    const Vec3f &operator += (const Vec3f &other);
    const Vec3f &operator -= (const Vec3f &other);
    
    float Magnitude() const;
    float MagnitudeSquared() const;
    Vec3f Normalize() const;
    float Dot(const Vec3f &other) const;
    Vec3f Cross(const Vec3f &other) const;
};

Vec3f operator * (float scale, const Vec3f &vec);
std::ostream &operator << (std::ostream &output, const Vec3f &vec);

#endif /* vec3f_h */
