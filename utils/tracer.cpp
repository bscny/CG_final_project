#include "tracer.h"
#include <cmath>

using namespace std;

float trace_shadow_ray(const Ray &r, float distance2light, const vector<Object *> &obj_list){
	// since ray.Dir is unit vector, t == distance
	float t;
	for(int obj_index = 0; obj_index < (int)obj_list.size(); obj_index ++){
		t = obj_list[obj_index]->hit(r, 0.01, distance2light);

		if(t > 0){
			// find intersact that is close enough!
			return 0;
		}
	}

	// nothing block the light
	return 1;
}

// for each ray, see the interaction with every scene objs
Vec3 color(const Ray &r, int bounce, const vector<Object *> &obj_list, const vector<Vec3> &light_sources, const vector<Vec3> &light_intensities){
	if(bounce < 0){
		return Vec3(0, 0, 0);
	}

	int record_index = -1;
	float t = INF;
	for(int obj_index = 0; obj_index < (int)obj_list.size(); obj_index ++){
		float ans = obj_list[obj_index]->hit(r, 0.001, t);

		if(ans > 0){
			// find intersact that is close enough!
			t = ans;
			record_index = obj_index;
		}
	}

	if(record_index == -1){
		// nothing hit! return default skybox BG
		t = 0.5 * (r.direction().y() + 1);

		return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1);
	}

	// apply shading result
	Vec3 P(r.point_at_parameter(t));
	Vec3 N(obj_list[record_index]->get_normal_at(P));
	Vec3 In(-r.Dir);

	// the reflection ray
	// R = In + 2 * cos * N
	Ray R(P, (2 * N * dot(N, In)) - In);

	// the refraction ray
	// see detailed in https://en.wikipedia.org/wiki/Snell%27s_law
	// and https://physics.stackexchange.com/questions/435512/snells-law-in-vector-form
	float eta = 1 / 1.458; // for glass
	float cos2 = sqrt(1 - pow(eta, 2) * (1 - pow(dot(N, r.Dir), 2)) );
	Ray T(P, eta * r.Dir + (eta * dot(N, r.Dir) - cos2) * N );

	Vec3 local_color;
	float occlude_lv = 0;

	for(int i = 0; i < (int)light_sources.size(); i ++){
		Vec3 L(unit_vector(light_sources[i] - P));

		if(dot(N, L) < 0){
			local_color += Vec3(0, 0, 0);
		}else{
			occlude_lv = trace_shadow_ray(Ray(P, L), sqrt(dot(light_sources[i] - P, light_sources[i] - P)), obj_list);
			local_color += occlude_lv * dot(N, L) * light_intensities[i];
		}
	}

	Vec3 reflected_color = color(R, bounce - 1, obj_list, light_sources, light_intensities);
	Vec3 refracted_color = color(T, bounce - 1, obj_list, light_sources, light_intensities);

	// return (1 - obj_list[record_index].get_w_r()) * local_color + obj_list[record_index].get_w_r() * reflected_color;
	return (1 - obj_list[record_index]->get_w_t()) * 
		   ( (1 - obj_list[record_index]->get_w_r()) * local_color + obj_list[record_index]->get_w_r() * reflected_color) +
		   obj_list[record_index]->get_w_t() * refracted_color;
}