#ifndef TRACER_H
#define TRACER_H

#include "constant.h"
#include "tools/vec3.h"
#include "tools/ray.h"
#include "tools/object.h"
#include "scene_objects/light.h"
#include <vector>

using namespace std;

float trace_shadow_ray(const Ray &r, float distance2light, const vector<Object *> &obj_list);

Vec3 trace_color_ray(const Ray &r, int bounce, const vector<Object *> &obj_list, const vector<Light> &lights, float current_reflective_index);

#endif