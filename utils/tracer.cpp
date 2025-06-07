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
Vec3 trace_color_ray(const Ray &r, int bounce, const vector<Object *> &obj_list, 
					 const vector<Light> &lights, int in_obj_index){
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
	
	// get the ratio of reflective index
	/*float current_refractive_index;
	float next_refractive_index;
	if(in_obj_index == -1){
		current_refractive_index = AIR_N;
		next_refractive_index = obj_list[record_index]->get_refractive_index();
	}else if(in_obj_index == record_index){
		current_refractive_index = obj_list[in_obj_index]->get_refractive_index();
		next_refractive_index = AIR_N;
	}else{
		current_refractive_index = obj_list[in_obj_index]->get_refractive_index();
		next_refractive_index = obj_list[record_index]->get_refractive_index();
	}

	// the refraction ray
	float eta = current_refractive_index / next_refractive_index;
	float cos2 = sqrt(1 - pow(eta, 2) * (1 - pow(dot(N, r.Dir), 2)) );
	Ray T(P, eta * r.Dir + (eta * dot(N, r.Dir) - cos2) * N );*/

	Vec3 local_color;
	float occlude_lv = 0;

	for(int i = 0; i < (int)lights.size(); i ++){
		Vec3 light_pos = lights[i].get_position();
		Vec3 L = unit_vector(light_pos - P);
		// the distence from the point to the point light
		float dist2 = dot(light_pos - P, light_pos - P);
		float dist = sqrt(dist2);

		float attenuation = 1.0f / (KC + KL * dist + KQ * dist2);

		if(dot(N, L) < 0){
			local_color += Vec3(0, 0, 0);
		}else{
			occlude_lv = trace_shadow_ray(Ray(P, L), sqrt(dot(lights[i].get_position() - P, lights[i].get_position() - P)), obj_list);
			local_color += occlude_lv * dot(N, L) * (attenuation * lights[i].get_intensity());
		}
	}

	Vec3 reflected_color;
	// Vec3 refracted_color;

	if(obj_list[record_index]->get_w_r() > 0.2){
		reflected_color = trace_color_ray(R, bounce - 1, obj_list, lights, in_obj_index);
	}

	/*if(obj_list[record_index]->get_w_t() > 0.3){
		refracted_color = trace_color_ray(T, bounce - 1, obj_list, lights, record_index);
	}*/

	return (1 - obj_list[record_index]->get_w_r()) * local_color + obj_list[record_index]->get_w_r() * reflected_color;
	/*return (1 - obj_list[record_index]->get_w_t()) * 
		   ( (1 - obj_list[record_index]->get_w_r()) * local_color + obj_list[record_index]->get_w_r() * reflected_color) +
		   obj_list[record_index]->get_w_t() * refracted_color;*/
}