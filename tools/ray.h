#ifndef RAYH
#define RAYH

#include "vec3.h"

class Ray
{
public:
    Vec3 O;   // Origin point
    Vec3 Dir; // Direction unit vector

    Ray();
    Ray(const Ray &r);
    Ray(const Vec3 origin, const Vec3 dir);

    Ray &operator=(const Ray &r);

    Vec3 origin() const;
    Vec3 direction() const;
    Vec3 point_at_parameter(float t) const;
};

#endif
