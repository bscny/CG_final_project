#include "sphere.h"
#include <cmath>
#include <iostream>

// the big 3
Sphere::Sphere() : Object() {
    center = Vec3(0, 0, 0);
    radius = 1;
}

Sphere::Sphere(const Sphere &in) : Object(in) {
    center = in.center;
    radius = in.radius;
}

Sphere Sphere::operator= (const Sphere &in){
    Object::operator= (in);

    center = in.center;
    radius = in.radius;

    return *this;
}

// other constructor
Sphere::Sphere(Vec3 c, float r) : Object() {
    center = c;
    radius = r;
}

Sphere::Sphere(Vec3 c, float r, float in_w_r) : Object(in_w_r) {
    center = c;
    radius = r;
}

Sphere::Sphere(Vec3 c, float r, float in_w_r, float in_w_t) : Object(in_w_r, in_w_t) {
    center = c;
    radius = r;
}

Sphere::Sphere(Vec3 c, float r, float in_w_r, float in_w_t, float in_refractive_index) : Object(in_w_r, in_w_t, in_refractive_index) {
    center = c;
    radius = r;
}

// get the t value for the given ray, if t < max_t, return the t
float Sphere::hit(const Ray &ray, float min_t, float max_t){
    // O is origin of the ray, D is direction vector, C is sphere center. They are all vectors
    // || O + tD - C || = r
	// (O dot O) + 2t(O dot D) - 2(O dot C) + t^2(D dot D) - 2t(D dot C) + C dot C = r^2
	// t^2 + [2(O dot D) - 2(D dot C)]*t + [(O dot O) - 2(O dot C) + (C dot C) - r^2] = 0
	float a = 1;
	float b = 2 * (dot(ray.origin(), ray.direction()) - dot(ray.direction(), center));
	float c = dot(ray.origin(), ray.origin()) - 2 * dot(ray.origin(), center) + dot(center, center) - radius*radius;
	float b_sqr = pow(b, 2);
	
	if(b_sqr >= 4 * a * c){
		// Hit!!
		// since we want to "look at" the surface, we use subtraction
		float t = (-b - sqrt(b_sqr - 4 * a * c)) / (2 * a);

        if(t < max_t && t > min_t){
            // update the recorded index
            return t;
        }else{
            return -1;
        }
	}else{
		return -1;
	}
}

// get the normal vector (unit) ata given point P
Vec3 Sphere::get_normal_at(Vec3 P) const {
    // check if P is on the sphere
    if(abs(dot(P - center, P - center) - pow(radius, 2)) > 0.001){
        std::cout << "the given P is not on the sphere!!!!";
    }

    return unit_vector(P - center);
}