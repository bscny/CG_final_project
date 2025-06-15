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


	float min_x = -0.5f;
	float max_x = 0.5f;
   	float min_y = -0.5f;
    float max_y = 0.5f;
    float min_z = -2.5f;
    float max_z = -1.5f;
    
    // add small room floor
    obj_list.push_back(new Triangle(Vec3(min_x, min_y, min_z), Vec3(max_x, min_y, min_z), Vec3(min_x, min_y, max_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(max_x, min_y, min_z), Vec3(max_x, min_y, max_z), Vec3(min_x, min_y, max_z), 0, 0));
    // add small room ceiling
    obj_list.push_back(new Triangle(Vec3(min_x, max_y, max_z), Vec3(max_x, max_y, max_z), Vec3(min_x, max_y, min_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(max_x, max_y, max_z), Vec3(max_x, max_y, min_z), Vec3(min_x, max_y, min_z), 0, 0));
    // add small room walls on the back
    obj_list.push_back(new Triangle(Vec3(max_x, max_y, max_z), Vec3(min_x, max_y, max_z), Vec3(min_x, min_y, max_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(min_x, min_y, max_z), Vec3(max_x, min_y, max_z), Vec3(max_x, max_y, max_z), 0, 0));
    // add small room walls on the front
    obj_list.push_back(new Triangle(Vec3(min_x, max_y, min_z), Vec3(max_x, max_y, min_z), Vec3(max_x, min_y, min_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(max_x, min_y, min_z), Vec3(min_x, min_y, min_z), Vec3(min_x, max_y, min_z), 0, 0));
    // add small room walls on the left
    obj_list.push_back(new Triangle(Vec3(min_x, max_y, max_z), Vec3(min_x, max_y, min_z), Vec3(min_x, min_y, min_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(min_x, min_y, min_z), Vec3(min_x, min_y, max_z), Vec3(min_x, max_y, max_z), 0, 0));
    // add small room walls on the right
    obj_list.push_back(new Triangle(Vec3(max_x, max_y, min_z), Vec3(max_x, max_y, max_z), Vec3(max_x, min_y, max_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(max_x, min_y, max_z), Vec3(max_x, min_y, min_z), Vec3(max_x, max_y, min_z), 0, 0));

	// add random small boxes
    // for (int i = 0; i < 15; i++) {
 
    //     float scale = get_random(0.2f, 0.5f); 
        
    //     float center_x = get_random(-3.0f, 3.0f);
    //     float center_y = get_random(0.0f, 2.5f);
    //     float center_z = get_random(-9.0f, 1.0f);
        
    //     float half_width = 0.7f * scale;
    //     float half_height = 0.75f * scale;
    //     float half_depth = 1.0f * scale;
        
    //     float floor_y = center_y - half_height;
    //     float ceiling_y = center_y + half_height;
    //     float left_x = center_x - half_width;
    //     float right_x = center_x + half_width;
    //     float back_z = center_z + half_depth;
    //     float front_z = center_z - half_depth;
        
    //     // add small box floor
    //     obj_list.push_back(new Triangle(Vec3(left_x, floor_y, front_z), Vec3(right_x, floor_y, front_z), Vec3(left_x, floor_y, back_z), 0, 0));
    //     obj_list.push_back(new Triangle(Vec3(right_x, floor_y, front_z), Vec3(right_x, floor_y, back_z), Vec3(left_x, floor_y, back_z), 0, 0));
    //     // add small box ceiling
    //     obj_list.push_back(new Triangle(Vec3(left_x, ceiling_y, back_z), Vec3(right_x, ceiling_y, back_z), Vec3(left_x, ceiling_y, front_z), 0, 0));
    //     obj_list.push_back(new Triangle(Vec3(right_x, ceiling_y, back_z), Vec3(right_x, ceiling_y, front_z), Vec3(left_x, ceiling_y, front_z), 0, 0));
        
    //     // add small box walls on the back
    //     obj_list.push_back(new Triangle(Vec3(right_x, ceiling_y, back_z), Vec3(left_x, ceiling_y, back_z), Vec3(left_x, floor_y, back_z), 0, 0));
    //     obj_list.push_back(new Triangle(Vec3(left_x, floor_y, back_z), Vec3(right_x, floor_y, back_z), Vec3(right_x, ceiling_y, back_z), 0, 0));
    //     // add small box walls on the front
    //     obj_list.push_back(new Triangle(Vec3(left_x, ceiling_y, front_z), Vec3(right_x, ceiling_y, front_z), Vec3(right_x, floor_y, front_z), 0, 0));
    //     obj_list.push_back(new Triangle(Vec3(right_x, floor_y, front_z), Vec3(left_x, floor_y, front_z), Vec3(left_x, ceiling_y, front_z), 0, 0));
    //     // add small box walls on the left
    //     obj_list.push_back(new Triangle(Vec3(left_x, ceiling_y, back_z), Vec3(left_x, ceiling_y, front_z), Vec3(left_x, floor_y, front_z), 0, 0));
    //     obj_list.push_back(new Triangle(Vec3(left_x, floor_y, front_z), Vec3(left_x, floor_y, back_z), Vec3(left_x, ceiling_y, back_z), 0, 0));
    //     // add small box walls on the right
    //     obj_list.push_back(new Triangle(Vec3(right_x, ceiling_y, front_z), Vec3(right_x, ceiling_y, back_z), Vec3(right_x, floor_y, back_z), 0, 0));
    //     obj_list.push_back(new Triangle(Vec3(right_x, floor_y, back_z), Vec3(right_x, floor_y, front_z), Vec3(right_x, ceiling_y, front_z), 0, 0));
    // }
	
}

void create_scene_objects(vector<Object *> &obj_list) {
	// add main sphere
	obj_list.push_back(new Sphere(Vec3(0, 0, -2), 0.5, 0, 0, GLASS_N));
	obj_list.push_back(new Sphere(Vec3(1, 0, -1.75), 0.5, 0.6, 0, WATER_N));
	obj_list.push_back(new Sphere(Vec3(-1.5, 0, -3), 0.5, 0.8, 0, DIAMOND_N));

	// add tetrahedron
	// Vec3 move(2, 0.5, -4.1);
	// Vec3 v1 = 0.5 * Vec3( 1,  1,  1) + move;
	// Vec3 v2 = 0.5 * Vec3(-1, -1,  1) + move;
	// Vec3 v3 = 0.5 * Vec3(-1,  1, -1) + move;
	// Vec3 v4 = 1 * Vec3( 1, -1, -1) + move;

	// add_tetrahedron(obj_list, v1, v2, v3, v4, 0, 0);

	// for (int i = 0; i < 30; i++) {
	// 	float scale = get_random(0.1f, 0.4f); // 0.1 到 0.4
		
	// 	float max_offset = scale * sqrt(3); 
		
	// 	float safe_x_min = -4.0f + max_offset;
	// 	float safe_x_max = 4.0f - max_offset;
	// 	float safe_y_min = -0.55f + max_offset;
	// 	float safe_y_max = 4.0f - max_offset;
	// 	float safe_z_min = -10.0f + max_offset;
	// 	float safe_z_max = 2.0f - max_offset;
		
	// 	float x = get_random(safe_x_min, safe_x_max);
	// 	float y = get_random(safe_y_min, safe_y_max);
	// 	float z = get_random(safe_z_min, safe_z_max);
		
	// 	Vec3 move(x, y, z);
	// 	Vec3 v1 = scale * Vec3( 1,  1,  1) + move;
	// 	Vec3 v2 = scale * Vec3(-1, -1,  1) + move;
	// 	Vec3 v3 = scale * Vec3(-1,  1, -1) + move;
	// 	Vec3 v4 = scale * Vec3( 1, -1, -1) + move;
		
	// 	add_tetrahedron(obj_list, v1, v2, v3, v4, 0, 0);
	// }

	// // add random objs
	for (int i = 0; i < 15; i++) {
		float xr = get_random(-2, 2);
		float zr = get_random(-1.5, 1.5);
		float r1 = get_random(0, 1);
		// float r2 = get_random(0, 1) - 0.5;
		// if (r2 < 0){
		// 	r2 = 0;
		// }
		obj_list.push_back(new Sphere(Vec3(xr, -0.45, zr-2), 0.05, r1, 0));
	}
}

void create_scene_lights(vector<Light> &lights) {
    for (int i = 0; i < 1200; i++) {
		float xr = get_random(-1.5, 1.5);
		float yr = get_random(0, 2);
		float zr = get_random(-6, 1);
        
        float max_intensity = 0.05f;
		float c = get_random(0, max_intensity);
        if(i < 200){
            // pure red 
            lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, 0)));
        }else if(i < 400){
            // red + green 
            lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c, 0)));
        }else if(i < 600){
			// pure green
            lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, 0)));
        }else if(i < 800){
            // green + blue 
            lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, c)));
        }else if(i < 1000){
            // pure blue
            lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, 0, c)));
        }else{
            // red + blue 
            lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, c)));
		}
    }
	// for (int i = 0; i < 10; i++) {
	// 	float xr = get_random(-1.5, 1.5);
	// 	float zr = get_random(-6, 1);
	// 	float yr = get_random(0, 2);

	// 	float max_intensity = 0.5f;
	// 	float c = get_random(0, max_intensity);
	// 	if(xr < -1.5f + 1 * (3.0f / 6.0f)){
	// 		// pure red
	// 		lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, 0)));
	// 	}else if(xr < -1.5f + 2 * (3.0f / 6.0f)){
	// 		// red + green
	// 		lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c, 0)));
	// 	}else if(xr < -1.5f + 3 * (3.0f / 6.0f)){
	// 		// pure green
	// 		lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, 0)));
	// 	}else if(xr < -1.5f + 4 * (3.0f / 6.0f)){
	// 		// green + blue
	// 		lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, c)));
	// 	}else if(xr < -1.5f + 5 * (3.0f / 6.0f)){
	// 		// pure blue
	// 		lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, 0, c)));
	// 	}else{
	// 		// red + blue
	// 		lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, c)));
	// 	}
	// }
}

void create_scene_light_grids(vector<LightGrid> &lgs, vector<Vec3> &bounds) {
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
void create_scene(vector<Object *> &obj_list, vector<Vec3> &camera_position, vector<LightGrid> &lgs) {
	create_scene_objects(obj_list);
	// create_scene_light_grids(lgs, bounds);
}

// for light
void create_scene(vector<Object *> &obj_list, vector<Vec3> &camera_position, vector<Light> &lights) {
	create_scene_objects(obj_list);
	add_box(obj_list);
	create_scene_lights(lights);
}