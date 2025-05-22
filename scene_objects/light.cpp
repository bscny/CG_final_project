#include "light.h"

// the big 3
Light::Light() {
    position = Vec3(0, 0, 0);
    intensity = Vec3(0, 0, 0);
}

Light::Light(const Light &in) {
    position = in.position;
    intensity = in.intensity;
}

Light Light::operator= (const Light &in) {
    position = in.position;
    intensity = in.intensity;

    return *this;
}

// other constructor
Light::Light(Vec3 P, Vec3 I) {
    position = P;
    intensity = I;
}

Light::Light(float px, float py, float pz, float r, float g, float b) {
    position.e[0] = px;
    position.e[1] = py;
    position.e[2] = pz;

    intensity.e[0] = r;
    intensity.e[1] = g;
    intensity.e[2] = b;
}

// getters
Vec3 Light::get_position() const {
    return position;
}

Vec3 Light::get_intensity() const {
    return intensity;
}

// setters
void Light::set_position(Vec3 P) {
    position = P;
}

void Light::set_intensity(Vec3 I) {
    intensity = I;
}