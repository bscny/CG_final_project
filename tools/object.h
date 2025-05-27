#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.h"
#include "ray.h"

class Object{
    public:
        // big 3
        Object();
        Object(const Object &in);
        void operator= (const Object &in);

        // other constructor
        Object(float in_w_r);
        Object(float in_w_r, float in_w_t);

        // virtuals
        virtual float hit(const Ray &ray, float min_t, float max_t) = 0;
        virtual Vec3 get_normal_at(Vec3 P) const = 0;

        // getters
        float get_w_r() const;
        float get_w_t() const;
        float get_reflective_index() const;

        // setters
        void set_w_r(float in);
        void set_w_t(float in);
        void set_reflective_index(float in);
    
    private:
        float w_r;
        float w_t;

        // material reflectivity 
        float reflective_index; 
};

#endif