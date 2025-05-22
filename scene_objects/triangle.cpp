#include "triangle.h"
#include <cmath>
#include <iostream>

// the big 3
Triangle::Triangle() : Object() {
    P1 = Vec3(0, 2 / 3 * pow(3, 0.5), 0);
    P2 = Vec3(-1, -1 / 3 * pow(3, 0.5), 0);
    P3 = Vec3(1, -1 / 3 * pow(3, 0.5), 0);

    V1 = P2 - P1;
    V2 = P3 - P1;
}

Triangle::Triangle(const Triangle &in) : Object(in) {
    P1 = in.P1;
    P2 = in.P2;
    P3 = in.P3;

    V1 = in.V1;
    V2 = in.V2;
}

Triangle Triangle::operator= (const Triangle &in){
    Object::operator= (in);

    P1 = in.P1;
    P2 = in.P2;
    P3 = in.P3;

    V1 = in.V1;
    V2 = in.V2;

    return *this;
}

// other constructor
Triangle::Triangle(Vec3 in1, Vec3 in2, Vec3 in3) : Object() {
    P1 = in1;
    P2 = in2;
    P3 = in3;

    V1 = P2 - P1;
    V2 = P3 - P1;
}

Triangle::Triangle(Vec3 in1, Vec3 in2, Vec3 in3, float in_w_r) : Object(in_w_r) {
    P1 = in1;
    P2 = in2;
    P3 = in3;

    V1 = P2 - P1;
    V2 = P3 - P1;
}

Triangle::Triangle(Vec3 in1, Vec3 in2, Vec3 in3, float in_w_r, float in_w_t) : Object(in_w_r, in_w_t) {
    P1 = in1;
    P2 = in2;
    P3 = in3;

    V1 = P2 - P1;
    V2 = P3 - P1;
}

// get the t value for the given ray, if t < max_t, return the t
float Triangle::hit(const Ray &ray, float min_t, float max_t){
    // reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    // put it in simple words, we first exam if the ray touches the plane
    // if touches, we exam if it touches the triangle
    // for any point P on the plane, we have: P = V1 + u(V2 - V1) + v(V3 - V1), where u, v are numbers
    // now, substitute P for O + tD, where O is the origin of the ray, D is the direction
    // we have: O - V1 = -tD + u(V2 - V1) + v(V3 - V1), 3 unknown variable, 3 equations(x, y, z)
    Vec3 rayXv2(cross(ray.Dir, V2));
    float det = dot(V1, rayXv2);

    if(abs(det) < 0.0001){
        // This ray is parallel to this triangle plane
        return -1;
    }

    float inv_det = 1 / det;
    Vec3 S = ray.O - P1;
    float u = inv_det * dot(S, rayXv2);

    if(u < -0.0001 || u > 1.0001){
        return -1;
    }

    Vec3 sXv1(cross(S, V1));
    float v = inv_det * dot(ray.Dir, sXv1);

    if(v < -0.0001 || u + v > 1.0001){
        return -1;
    }

    // At this stage we can compute t to find out where the intersection point is on the line
    float t = inv_det * dot(V2, sXv1);

    if(t > 0.0001){
        if(t < max_t && t > min_t){
            // update the recorded index
            return t;
        }else{
            return -1;
        }
    }else{
        // this means the ray touches the triangle from behind
        return -1;
    }
}

// get the normal vector (unit) ata given point P
Vec3 Triangle::get_normal_at(Vec3 P) const {
    return unit_vector(cross(V1, V2));
}