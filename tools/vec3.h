#ifndef VEC3H
#define VEC3H

#include <iostream>

class Vec3
{
public:
    float e[3];

    Vec3();
    Vec3(float e0, float e1, float e2);
    Vec3(const Vec3 &v);

    float x() const;
    float y() const;
    float z() const;
    float r() const;
    float g() const;
    float b() const;

    Vec3 &operator=(const Vec3 &v);
    const Vec3 &operator+() const;
    Vec3 operator-() const;
    float operator[](int i) const;
    float &operator[](int i);

    Vec3 &operator+=(const Vec3 &v2);
    Vec3 &operator-=(const Vec3 &v2);
    Vec3 &operator*=(const Vec3 &v2);
    Vec3 &operator/=(const Vec3 &v2);
    Vec3 &operator*=(float t);
    Vec3 &operator/=(float t);

    float length() const;
    float squared_length() const;
    Vec3 &make_unit_vector();
};

// I/O
std::istream &operator>>(std::istream &is, Vec3 &t);
std::ostream &operator<<(std::ostream &os, const Vec3 &t);

// Arithmetic
Vec3 operator+(const Vec3 &v1, const Vec3 &v2);
Vec3 operator-(const Vec3 &v1, const Vec3 &v2);
Vec3 operator*(const Vec3 &v1, const Vec3 &v2);
Vec3 operator/(const Vec3 &v1, const Vec3 &v2);
Vec3 operator*(float t, const Vec3 &v);
Vec3 operator/(Vec3 v, float t);
Vec3 operator*(const Vec3 &v, float t);

// Vector operations
float dot(const Vec3 &v1, const Vec3 &v2);
Vec3 cross(const Vec3 &v1, const Vec3 &v2);
Vec3 unit_vector(Vec3 v);

#endif
