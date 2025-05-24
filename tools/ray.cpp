#include "ray.h"

// Default constructor
Ray::Ray()
    : O(Vec3(0, 0, 0)), Dir(Vec3(0, 0, 0)) {}

// Copy constructor
Ray::Ray(const Ray &r)
    : O(r.O), Dir(r.Dir) {}

// Parameterized constructor
Ray::Ray(const Vec3 origin, const Vec3 dir)
    : O(origin), Dir(unit_vector(dir)) {}

// Assignment operator
Ray &Ray::operator=(const Ray &r)
{
    O = r.O;
    Dir = r.Dir;
    return *this;
}

// Get origin
Vec3 Ray::origin() const
{
    return O;
}

// Get direction
Vec3 Ray::direction() const
{
    return Dir;
}

// Compute point along the ray
Vec3 Ray::point_at_parameter(float t) const
{
    return O + t * Dir;
}
