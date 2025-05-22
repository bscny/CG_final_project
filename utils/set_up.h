#ifndef SETUP_H
#define SETUP_H

#include "tools/object.h"
#include "tools/vec3.h"
#include "scene_objects/mesh.h"
#include "scene_objects/triangle.h"
#include "scene_objects/sphere.h"

#include <vector>

using namespace std;

void create_scene_objects(vector<Object *> &obj_list);
void create_scene_lights(vector<Vec3> &light_sources, vector<Vec3> &light_intensities);

#endif