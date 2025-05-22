#ifndef SETUP_H
#define SETUP_H

#include "tools/object.h"
#include "tools/vec3.h"
#include "scene_objects/light.h"
#include "scene_objects/mesh.h"
#include "scene_objects/triangle.h"
#include "scene_objects/sphere.h"

#include <vector>

using namespace std;

void create_scene_objects(vector<Object *> &obj_list);
void create_scene_lights(vector<Light> &lights);

#endif