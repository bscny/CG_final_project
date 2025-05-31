#include "set_up.h"
#include "constant.h"
using namespace std;


void create_scene_objects(vector<Object *> &obj_list){
    // add floor
	// obj_list.push_back(new Sphere(Vec3(0, -100.5, -2), 100));
	obj_list.push_back(new Triangle(Vec3(-100, -0.55, 100), Vec3(100, -0.55, 100), Vec3(-100, -0.55, -100), 0, 0));
	obj_list.push_back(new Triangle(Vec3(100, -0.55, 100), Vec3(100, -0.55, -100), Vec3(-100, -0.55, -100), 0, 0));

	// add main sphere
	obj_list.push_back(new Sphere(Vec3(0, 0, -2), 0.5, 0, 0, GLASS_N));
	obj_list.push_back(new Sphere(Vec3(1, 0, -1.75), 0.5, 0.3, 0, WATER_N));
	obj_list.push_back(new Sphere(Vec3(-1, 0, -2.25), 0.5, 0.8, 0, DIAMOND_N));

	// add tetrahedron
	/*Vec3 move(0, 0, -3);
	Vec3 v1 = 1 * Vec3( 1,  1,  1) + move;
	Vec3 v2 = 1 * Vec3(-1, -1,  1) + move;
	Vec3 v3 = 1 * Vec3(-1,  1, -1) + move;
	Vec3 v4 = 1 * Vec3( 1, -1, -1) + move;

	add_tetrahedron(obj_list, v1, v2, v3, v4, 0, 0);*/

	// add random objs
	srand(1234);
	for (int i = 0; i < 48; i++) {
		float xr = ((float)rand() / (float)(RAND_MAX)) * 6.0f - 3.0f;
		float zr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;
		float r1 = ((float)rand() / (float)(RAND_MAX));
		// float r2 = ((float)rand() / (float)(RAND_MAX)) - 0.5;
		// if (r2 < 0){
		// 	r2 = 0;
		// }
		obj_list.push_back(new Sphere(Vec3(xr, -0.45, zr-2), 0.05, r1, 0));
	}
}

void create_scene_lights(vector<Light> &lights){
    // set up lights
	lights.push_back(Light(Vec3(-10, 10, 0), Vec3(0.5, 0, 0)));
	lights.push_back(Light(Vec3(10, 10, 0), Vec3(0.25, 0.25, 0.5)));
	lights.push_back(Light(Vec3(0, 10, 10), Vec3(0, 0.5, 0)));
	lights.push_back(Light(Vec3(0, 10, -10), Vec3(0.25, 0.25, 0.5)));
}