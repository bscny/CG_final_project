#ifndef TRACER_H
#define TRACER_H

#include "constant.h"
#include "tools/vec3.h"
#include "tools/ray.h"
#include "tools/object.h"
#include "scene_objects/light.h"
#include "scene_objects/light_grid.h"
#include <vector>

using namespace std;

float trace_shadow_ray(const Ray &r, float distance2light, const vector<Object *> &obj_list);

float blending(int max_lv, int curr_lv, float d, float r);

Vec3 trace_color_ray(const Ray &r, int bounce, const vector<Object *> &obj_list, const vector<Light> &lights, int in_obj_index);

Vec3 trace_color_ray_lg(const Ray &r, int bounce, const vector<Object *> &obj_list, const vector<LightGrid> &lgs, int in_obj_index);

#endif