#ifndef INSERT_LG_H
#define INSERT_LG_H

#include "tools/vec3.h"
#include "scene_objects/light_grid.h"

#include <vector>

using namespace std;

void insert_influenced_grid_vertices(LightGrid &lg, Vec3 light_pos, Vec3 light_intensity, Vec3 grid_org, float h);

#endif