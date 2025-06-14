#ifndef SETUP_H
#define SETUP_H

#include "tools/object.h"
#include "tools/vec3.h"
#include "scene_objects/light.h"
#include "scene_objects/mesh.h"
#include "scene_objects/triangle.h"
#include "scene_objects/sphere.h"
#include "scene_objects/light_grid.h"

#include <vector>

using namespace std;

void create_scene_objects(vector<Object *> &obj_list, vector<Vec3> &bounds);

void create_scene_lights(vector<Light> &lights, vector<Vec3> &bounds);

void create_scene_light_grids(vector<LightGrid> &lgs, vector<Vec3> &bounds);

void create_scene(vector<Object *> &obj_list,  vector<Vec3> &camera_position, vector<LightGrid> &lgs);
void create_scene(vector<Object *> &obj_list,  vector<Vec3> &camera_position, vector<Light> &lights);

#endif