#ifndef MESH_H
#define MESH_H

#include "tools/vec3.h"
#include "tools/object.h"
#include <vector>

void add_tetrahedron(std::vector<Object *> &a, Vec3 V1, Vec3 V2, Vec3 V3, Vec3 V4, float w_r, float w_t);

#endif