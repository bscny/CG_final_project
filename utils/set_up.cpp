#include <random>
#include <vector>
#include <cstdio>

#include "set_up.h"
#include "constant.h"
#include "utils/insert_lg.h"
using namespace std;

float get_random(float lower, float upper) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<float> dist(lower, upper);

    return dist(gen);
}

void add_box(vector<Object *> &obj_list) {
	// add floor
	obj_list.push_back(new Triangle(Vec3(-100, -0.55, 100), Vec3(100, -0.55, 100), Vec3(-100, -0.55, -100), 0, 0));
	obj_list.push_back(new Triangle(Vec3(100, -0.55, 100), Vec3(100, -0.55, -100), Vec3(-100, -0.55, -100), 0, 0));
	// add ceiling
	obj_list.push_back(new Triangle(Vec3(-100, 4, -100), Vec3(100, 4, -100), Vec3(-100, 4, 100), 0, 0));
	obj_list.push_back(new Triangle(Vec3(100, 4, -100), Vec3(100, 4, 100), Vec3(-100, 4, 100), 0, 0));

	// add walls on the back
	obj_list.push_back(new Triangle(Vec3(100, 100, -10), Vec3(-100, 100, -10), Vec3(-100, -100, -10), 0, 0));
	obj_list.push_back(new Triangle(Vec3(-100, -100, -10), Vec3(100, -100, -10), Vec3(100, 100, -10), 0, 0));
	// add walls on the front
	obj_list.push_back(new Triangle(Vec3(-100, 100, 2), Vec3(100, 100, 2), Vec3(100, -100, 2), 0, 0));
	obj_list.push_back(new Triangle(Vec3(100, -100, 2), Vec3(-100, -100, 2), Vec3(-100, 100, 2), 0, 0));
	// add walls on the left
	obj_list.push_back(new Triangle(Vec3(-4, 100, -100), Vec3(-4, 100, 100), Vec3(-4, -100, 100), 0, 0));
	obj_list.push_back(new Triangle(Vec3(-4, -100, 100), Vec3(-4, -100, -100), Vec3(-4, 100, -100), 0, 0));
	// add walls on the right
	obj_list.push_back(new Triangle(Vec3(4, 100, 100), Vec3(4, 100, -100), Vec3(4, -100, -100), 0, 0));
	obj_list.push_back(new Triangle(Vec3(4, -100, -100), Vec3(4, -100, 100), Vec3(4, 100, 100), 0, 0));
}

// add the the light with the ball cover, its function is to be the light source
// there will be a lot of lights on the air ball face, and spread the lights to 
// light of the scene, and the light in this ball must me the same color light
// , which around over on the AIR ball  
void add_small_ball_with_light(vector<Object *> &obj_list, vector<Light> &lights){
    float sphere_radius = 0.025f;
	int num_sphere = 67;
	int num_lights_on_sphere = 18;
	float light_pos_offset = 0.5f * sphere_radius;
	float max_intensity = 0.05f;

    for (int i = 0; i < num_sphere; i++) {
        // Random position within bounds
        float xr = get_random(-2, 2);
		float yr = get_random(-0.5, 1);
		float zr = get_random(-6, -1);
        
        // Create the sphere object (air ball with refractive index)
		if (zr < -2) {
			obj_list.push_back(new Sphere(Vec3(xr, yr, zr), sphere_radius, 0, 0, AIR_N));
		}

        // Choose ONE color based on sphere's x-position (same as create_scene_light_grids)
        float x_range = 4;
        float x_ratio = (xr + 2) / x_range;
        
        Vec3 base_color;
        if(x_ratio < 1.0f / 6.0f){
            // pure red
            base_color = Vec3(1, 0, 0);
        }else if(x_ratio < 2.0f / 6.0f){
            // red + green (yellow)
            base_color = Vec3(1, 1, 0);
        }else if(x_ratio < 3.0f / 6.0f){
            // pure green
            base_color = Vec3(0, 1, 0);
        }else if(x_ratio < 4.0f / 6.0f){
            // green + blue (cyan)
            base_color = Vec3(0, 1, 1);
        }else if(x_ratio < 5.0f / 6.0f){
            // pure blue
            base_color = Vec3(0, 0, 1);
        }else{
            // red + blue (magenta)
            base_color = Vec3(1, 0, 1);
        }

		// The 18 directions of the VPls around the sphere
		float directions[num_lights_on_sphere][3] = {
			// 6 cardinal directions
			{1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
			{0, -1, 0}, {0, 0, 1}, {0, 0, -1},
			// 12 edge directions
			{0.707f, 0.707f, 0}, {0.707f, -0.707f, 0},
			{-0.707f, 0.707f, 0}, {-0.707f, -0.707f, 0},
			{0.707f, 0, 0.707f}, {0.707f, 0, -0.707f},
			{-0.707f, 0, 0.707f}, {-0.707f, 0, -0.707f},
			{0, 0.707f, 0.707f}, {0, 0.707f, -0.707f},
			{0, -0.707f, 0.707f}, {0, -0.707f, -0.707f}
		};

        // Distribute lights uniformly on the sphere surface
        // All lights on this sphere will have the same color but different intensities
        for (int j = 0; j < num_lights_on_sphere; j++) {
            // Use uniform distribution on sphere surface
            // float theta = get_random(0, 2 * M_PI);  // Azimuthal angle
            // float phi = acos(get_random(-1, 1));    // Polar angle (uniform on sphere surface)

            // Place lights exactly on the sphere surface
            float light_offset = sphere_radius + light_pos_offset;  // Small gap to prevent noise
            // float light_x = xr + light_offset * sin(phi) * cos(theta);
            // float light_y = yr + light_offset * sin(phi) * sin(theta);
            // float light_z = zr + light_offset * cos(phi);
			float light_x = xr + light_offset * directions[j][0];
            float light_y = yr + light_offset * directions[j][1];
            float light_z = zr + light_offset * directions[j][2];


			float light_intensity = get_random(0, max_intensity);
            Vec3 light_color = base_color * light_intensity;

            lights.push_back(Light(Vec3(light_x, light_y, light_z), light_color));
        }
    }
}

// Add glowing spheres with lights that contribute to light grids
// Each sphere acts as a light source with many surface lights of the same color
void add_small_ball_with_lightgrids(vector<Object *> &obj_list, vector<LightGrid> &lgs){
    float sphere_radius = 0.025f;
	int num_sphere = 67;
	int num_lights_on_sphere = 18;
	float light_pos_offset = 0.5f * sphere_radius;
	float max_intensity = 0.05f;

	// for lgs
	float off_set = 0.1;
	Vec3 max_bound(2 + off_set, 1 + off_set, -1 + off_set);
	Vec3 min_bound(-2 - off_set, -0.5 - off_set, -6 - off_set);

	int lv_num = 3;
	for (int i = 0; i <= lv_num; i ++) {
		// top right, bottom left
		lgs.push_back(LightGrid(max_bound, min_bound));
	}

    for (int i = 0; i < num_sphere; i++) {
        // Random position within bounds
        float xr = get_random(-2, 2);
		float yr = get_random(-0.5, 1);
		float zr = get_random(-6, -1);
        
        // Create the sphere object (air ball with refractive index)
        if (zr < -2) {
			obj_list.push_back(new Sphere(Vec3(xr, yr, zr), sphere_radius, 0, 0, AIR_N));
		}

        // Choose ONE color based on sphere's x-position (same as create_scene_light_grids)
        float x_range = 4;
        float x_ratio = (xr + 2) / x_range;

        Vec3 base_color;
        if(x_ratio < 1.0f / 6.0f){
            // pure red
            base_color = Vec3(1, 0, 0);
        }else if(x_ratio < 2.0f / 6.0f){
            // red + green (yellow)
            base_color = Vec3(1, 1, 0);
        }else if(x_ratio < 3.0f / 6.0f){
            // pure green
            base_color = Vec3(0, 1, 0);
        }else if(x_ratio < 4.0f / 6.0f){
            // green + blue (cyan)
            base_color = Vec3(0, 1, 1);
        }else if(x_ratio < 5.0f / 6.0f){
            // pure blue
            base_color = Vec3(0, 0, 1);
        }else{
            // red + blue (magenta)
            base_color = Vec3(1, 0, 1);
        }

		// The 18 directions of the VPls around the sphere
		float directions[num_lights_on_sphere][3] = {
			// 6 cardinal directions
			{1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
			{0, -1, 0}, {0, 0, 1}, {0, 0, -1},
			// 12 edge directions
			{0.707f, 0.707f, 0}, {0.707f, -0.707f, 0},
			{-0.707f, 0.707f, 0}, {-0.707f, -0.707f, 0},
			{0.707f, 0, 0.707f}, {0.707f, 0, -0.707f},
			{-0.707f, 0, 0.707f}, {-0.707f, 0, -0.707f},
			{0, 0.707f, 0.707f}, {0, 0.707f, -0.707f},
			{0, -0.707f, 0.707f}, {0, -0.707f, -0.707f}
		};

        for (int j = 0; j < num_lights_on_sphere; j++) {
            // float theta = get_random(0, 2 * M_PI);  // Azimuthal angle
            // float phi = acos(get_random(-1, 1));    // Polar angle (uniform on sphere surface)
            
            // Place lights slightly outside the sphere surface to avoid numerical issues
            float light_offset = sphere_radius + light_pos_offset;
            // float light_x = xr + light_offset * sin(phi) * cos(theta);
            // float light_y = yr + light_offset * sin(phi) * sin(theta);
            // float light_z = zr + light_offset * cos(phi);
			float light_x = xr + light_offset * directions[j][0];
            float light_y = yr + light_offset * directions[j][1];
            float light_z = zr + light_offset * directions[j][2];
            
			float light_intensity = get_random(0, max_intensity);
            Vec3 light_color = base_color * light_intensity;
            
			// get the influenced grid vertices for this light at each lv
			for (int i = 0; i <= lv_num; i ++) {
				insert_influenced_grid_vertices(lgs[i], Vec3(light_x, light_y, light_z), light_color, min_bound, H * pow(2, i));
			}
        }
    }

	// re-build the lg so that it's as balanced as possible
	for (int i = 0; i <= lv_num; i ++) {
		lgs[i].balance();
		
		cout << "for grid lv of: " << i << endl;
		cout << "	num of node: " << lgs[i].get_size() << endl;
		cout << "	num of repeation: " << lgs[i].get_repeation() << endl;
		cout << "	tree depth: " << lgs[i].get_depth() << endl;
	}
}

void create_scene_objects(vector<Object *> &obj_list){
	// add main sphere
	obj_list.push_back(new Sphere(Vec3(0, 0, -4), 0.5, 0, 0, GLASS_N));
	obj_list.push_back(new Sphere(Vec3(1, 0, -4), 0.5, 0.6, 0, WATER_N));
	obj_list.push_back(new Sphere(Vec3(-1, 0, -4), 0.5, 0.8, 0, DIAMOND_N));

	// add random objs
	// for (int i = 0; i < 40; i++) {
	// 	float xr = get_random(-2, 2);
	// 	float zr = get_random(-1.5, 1.5);
	// 	float r1 = get_random(0, 1);

	// 	obj_list.push_back(new Sphere(Vec3(xr, -0.45, zr-2), 0.05, r1, 0));
	// }
}


void create_scene_lights(vector<Light> &lights) {
	for (int i = 0; i < 1200; i++) {
		float xr = get_random(-1.5, 1.5);
		float yr = get_random(0, 2);
		float zr = get_random(-6, 1);

		float max_intensity = 0.05f;
		float c = get_random(0, max_intensity);
		if(xr < -1.5f + 1 * (3.0f / 6.0f)){
			// pure red
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, 0)));
		}else if(xr < -1.5f + 2 * (3.0f / 6.0f)){
			// red + green
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c, 0)));
		}else if(xr < -1.5f + 3 * (3.0f / 6.0f)){
			// pure green
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, 0)));
		}else if(xr < -1.5f + 4 * (3.0f / 6.0f)){
			// green + blue
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, c)));
		}else if(xr < -1.5f + 5 * (3.0f / 6.0f)){
			// pure blue
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, 0, c)));
		}else{
			// red + blue
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, c)));
		}
	}
}

void create_scene_light_grids(vector<LightGrid> &lgs) {
	lgs.clear();

	// create all levels of lg
	int lv_num = 3;
	for (int i = 0; i <= lv_num; i ++) {
		lgs.push_back(LightGrid(Vec3(1.5, 2, 1), Vec3(-1.5, 0, -6)));
	}

	// creating VPL
	for (int i = 0; i < 1200; i++) {
		float xr = get_random(-1.5, 1.5);
		float zr = get_random(-6, 1);
		float yr = get_random(0, 2);
		Vec3 I;

		float max_intensity = 0.05f;
		float c = get_random(0, max_intensity);
		if(xr < -1.5f + 1 * (3.0f / 6.0f)){
			// pure red
			I = Vec3(c, 0, 0);
		}else if(xr < -1.5f + 2 * (3.0f / 6.0f)){
			// red + green
			I = Vec3(c, c, 0);
		}else if(xr < -1.5f + 3 * (3.0f / 6.0f)){
			// pure green
			I = Vec3(0, c, 0);
		}else if(xr < -1.5f + 4 * (3.0f / 6.0f)){
			// green + blue
			I = Vec3(0, c, c);
		}else if(xr < -1.5f + 5 * (3.0f / 6.0f)){
			// pure blue
			I = Vec3(0, 0, c);
		}else{
			// red + blue
			I = Vec3(c, 0, c);
		}

		// get the influenced grid vertices for this light at each lv
		for (int i = 0; i <= lv_num; i ++) {
			insert_influenced_grid_vertices(lgs[i], Vec3(xr, yr, zr), I, Vec3(-1.5, 0, -6), H * pow(2, i));
		} 
	}

	// re-build the lg so that it's as balanced as possible
	for (int i = 0; i <= lv_num; i ++) {
		lgs[i].balance();
		
		cout << "for grid lv of: " << i << endl;
		cout << "	num of node: " << lgs[i].get_size() << endl;
		cout << "	num of repeation: " << lgs[i].get_repeation() << endl;
		cout << "	tree depth: " << lgs[i].get_depth() << endl;
	}
}

// for light grid
void create_scene(vector<Object *> &obj_list, vector<LightGrid> &lgs) {
	add_box(obj_list);
	create_scene_objects(obj_list);
	add_small_ball_with_lightgrids(obj_list, lgs);
	// create_scene_light_grids(lgs);
}

// for light
void create_scene(vector<Object *> &obj_list, vector<Light> &lights) {
	add_box(obj_list);
	create_scene_objects(obj_list);
	add_small_ball_with_light(obj_list, lights);
	// create_scene_lights(lights);
}