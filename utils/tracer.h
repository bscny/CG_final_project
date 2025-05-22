#ifndef TRACER_H
#define TRACER_H

#include "tools/vec3.h"
#include "tools/ray.h"
#include "constant.h"
#include "tools/object.h"
#include <vector>

using namespace std;

float trace_shadow_ray(const Ray &r, float distance2light, const vector<Object *> &obj_list);

Vec3 color(const Ray &r, int bounce, const vector<Object *> &obj_list, const vector<Vec3> &light_sources, const vector<Vec3> &light_intensities);

#endif