#ifndef SPHERE_H
#define SPHERE_H

#include "tools/vec3.h"
#include "tools/ray.h"
#include "tools/object.h"

class Sphere : public Object{
    public:
        // big 3
        Sphere();
        Sphere(const Sphere &in);
        Sphere operator= (const Sphere &in);

        // other constructor
        Sphere(Vec3 c, float r);
        Sphere(Vec3 c, float r, float in_w_r);
        Sphere(Vec3 c, float r, float in_w_r, float in_w_t);
        Sphere(Vec3 c, float r, float in_w_r, float in_w_t, float in_refractive_index);

        // virtuals
        virtual float hit(const Ray &ray, float min_t, float max_t);
        virtual Vec3 get_normal_at(Vec3 P) const;

    private:
        Vec3 center;
        float radius;
};

#endif