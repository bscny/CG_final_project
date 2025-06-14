#include <random>

#include "set_up.h"
#include "constant.h"
#include "utils/insert_lg.h"
using namespace std;

#define   KC   1.0
#define   KL   1
#define   KQ   1.2

// define light grid alpha for blending function
#define   ALPHA   1
#define   H   0.0625

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

void create_scene_objects(vector<Object *> &obj_list){
	// add main sphere
	obj_list.push_back(new Sphere(Vec3(0, 0, -2), 0.5, 0, 0, GLASS_N));
	obj_list.push_back(new Sphere(Vec3(1, 0, -1.75), 0.5, 0.6, 0, WATER_N));
	obj_list.push_back(new Sphere(Vec3(-1, 0, -2.25), 0.5, 0.8, 0, DIAMOND_N));

	// add tetrahedron
	/*Vec3 move(0, 0, -3);
	Vec3 v1 = 1 * Vec3( 1,  1,  1) + move;
	Vec3 v2 = 1 * Vec3(-1, -1,  1) + move;
	Vec3 v3 = 1 * Vec3(-1,  1, -1) + move;
	Vec3 v4 = 1 * Vec3( 1, -1, -1) + move;

	add_tetrahedron(obj_list, v1, v2, v3, v4, 0, 0);*/

	// add random objs
	for (int i = 0; i < 40; i++) {
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

void create_scene_lights(vector<Light> &lights){
	for (int i = 0; i < 1200; i++) {
		float xr = get_random(-1.5, 1.5);
		float zr = get_random(-6, 1);
		float yr = get_random(0, 2);

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

	for (int i = 0; i <= lv_num; i ++) {
		cout << "for grid lv of: " << i << endl;
		cout << "	num of node: " << lgs[i].get_size() << endl;
		cout << "	num of repeation: " << lgs[i].get_repeation() << endl;
		cout << "	tree depth: " << lgs[i].get_depth() << endl;
	}
}

