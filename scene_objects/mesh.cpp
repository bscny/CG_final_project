#include "mesh.h"
#include "triangle.h"

// Assumes that Triangle constructor uses counter-clockwise winding for outward normal
void add_tetrahedron(std::vector<Object *> &obj_list, Vec3 V1, Vec3 V2, Vec3 V3, Vec3 V4, float w_r, float w_t) {
    // Face 1: base face (V2, V3, V4), opposite of V1
    if (dot(cross(V3 - V2, V4 - V2), V1 - V2) > 0)
        obj_list.push_back(new Triangle(V2, V4, V3, w_r, w_t)); // flip to make normal face outward
    else
        obj_list.push_back(new Triangle(V2, V3, V4, w_r, w_t));

    // Face 2: (V1, V4, V3), opposite of V2
    if (dot(cross(V4 - V1, V3 - V1), V2 - V1) > 0)
        obj_list.push_back(new Triangle(V1, V3, V4, w_r, w_t));
    else
        obj_list.push_back(new Triangle(V1, V4, V3, w_r, w_t));

    // Face 3: (V1, V2, V4), opposite of V3
    if (dot(cross(V2 - V1, V4 - V1), V3 - V1) > 0)
        obj_list.push_back(new Triangle(V1, V4, V2, w_r, w_t));
    else
        obj_list.push_back(new Triangle(V1, V2, V4, w_r, w_t));

    // Face 4: (V1, V3, V2), opposite of V4
    if (dot(cross(V3 - V1, V2 - V1), V4 - V1) > 0)
        obj_list.push_back(new Triangle(V1, V2, V3, w_r, w_t));
    else
        obj_list.push_back(new Triangle(V1, V3, V2, w_r, w_t));
}
