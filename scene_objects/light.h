#ifndef LIGHT_H
#define LIGHT_H

#include "tools/vec3.h"

class Light {
    public:
        // big 3
        Light();
        Light(const Light &in);
        Light operator= (const Light &in);

        // other constructor
        Light(Vec3 P, Vec3 I);
        Light(float px, float py, float pz, float r, float g, float b);

        // getters
        Vec3 get_position() const;
        Vec3 get_intensity() const;

        // setters
        void set_position(Vec3 P);
        void set_intensity(Vec3 I);
    private:
        Vec3 position;
        Vec3 intensity;
};

#endif