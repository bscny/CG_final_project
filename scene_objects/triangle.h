#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "tools/vec3.h"
#include "tools/ray.h"
#include "tools/object.h"

class Triangle : public Object{
    public:
        // big 3
        Triangle();
        Triangle(const Triangle &in);
        Triangle operator= (const Triangle &in);

        // other constructor
        Triangle(Vec3 in1, Vec3 in2, Vec3 in3);
        Triangle(Vec3 in1, Vec3 in2, Vec3 in3, float in_w_r);
        Triangle(Vec3 in1, Vec3 in2, Vec3 in3, float in_w_r, float in_w_t);
        Triangle(Vec3 in1, Vec3 in2, Vec3 in3, float in_w_r, float in_w_t, float in_refractive_index);
        
        // virtuals
        virtual float hit(const Ray &ray, float min_t, float max_t);
        virtual Vec3 get_normal_at(Vec3 P) const;

    private:
        Vec3 P1;
        Vec3 P2;
        Vec3 P3;

        // derived the edg vector from p1, p2, p3
        Vec3 V1;
        Vec3 V2;
};

#endif