#include "vec3.h"
#include <cmath>

// Constructors
Vec3::Vec3() : e{0, 0, 0} {}
Vec3::Vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}
Vec3::Vec3(const Vec3 &v) { e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2]; }

// Accessors
float Vec3::x() const { return e[0]; }
float Vec3::y() const { return e[1]; }
float Vec3::z() const { return e[2]; }
float Vec3::r() const { return e[0]; }
float Vec3::g() const { return e[1]; }
float Vec3::b() const { return e[2]; }

// Operator overloads
Vec3 &Vec3::operator=(const Vec3 &v)
{
    e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2];
    return *this;
}

const Vec3 &Vec3::operator+() const { return *this; }

Vec3 Vec3::operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

float Vec3::operator[](int i) const { return e[i]; }

float &Vec3::operator[](int i) { return e[i]; }

Vec3 &Vec3::operator+=(const Vec3 &v)
{
    e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2];
    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &v)
{
    e[0] -= v.e[0]; e[1] -= v.e[1]; e[2] -= v.e[2];
    return *this;
}

Vec3 &Vec3::operator*=(const Vec3 &v)
{
    e[0] *= v.e[0]; e[1] *= v.e[1]; e[2] *= v.e[2];
    return *this;
}

Vec3 &Vec3::operator/=(const Vec3 &v)
{
    e[0] /= v.e[0]; e[1] /= v.e[1]; e[2] /= v.e[2];
    return *this;
}

Vec3 &Vec3::operator*=(float t)
{
    e[0] *= t; e[1] *= t; e[2] *= t;
    return *this;
}

Vec3 &Vec3::operator/=(float t)
{
    float k = 1.0f / t;
    e[0] *= k; e[1] *= k; e[2] *= k;
    return *this;
}

float Vec3::length() const
{
    return std::sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
}

float Vec3::squared_length() const
{
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}

Vec3 &Vec3::make_unit_vector()
{
    float k = 1.0f / length();
    *this *= k;
    return *this;
}

// I/O
std::istream &operator>>(std::istream &is, Vec3 &t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

std::ostream &operator<<(std::ostream &os, const Vec3 &t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

// Arithmetic
Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

Vec3 operator*(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

Vec3 operator/(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

Vec3 operator*(float t, const Vec3 &v)
{
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

Vec3 operator/(Vec3 v, float t)
{
    return Vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

Vec3 operator*(const Vec3 &v, float t)
{
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

// Equality operators
bool operator==(const Vec3 &v1, const Vec3 &v2) {
    const float epsilon = 0.0000001;
    return (std::abs(v1.e[0] - v2.e[0]) < epsilon && 
            std::abs(v1.e[1] - v2.e[1]) < epsilon && 
            std::abs(v1.e[2] - v2.e[2]) < epsilon);
}

bool operator!=(const Vec3 &v1, const Vec3 &v2) {
    return !(v1 == v2);
}

// Vector operations
float dot(const Vec3 &v1, const Vec3 &v2)
{
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(
        v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1],
        v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2],
        v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]
    );
}

Vec3 unit_vector(Vec3 v)
{
    return v / v.length();
}
