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

void create_scene_objects(vector<Object *> &obj_list) {
	// add main sphere
	obj_list.push_back(new Sphere(Vec3(1.5, 0, -2.5), 0.5, 0, 0, DIAMOND_N));
	// obj_list.push_back(new Sphere(Vec3(1, 0, -1.75), 0.5, 0.6, 0, WATER_N));
	// obj_list.push_back(new Sphere(Vec3(-1.5, 0, -2.5), 0.5, 0.8, 0, DIAMOND_N));

	// add main box
	float min_x = -2.0f;
	float max_x = -1.0f;
   	float min_y = -0.5f;
    float max_y = 0.5f;
    float min_z = -3.5f;
    float max_z = -2.5f;
    float box_r_t = 0.5f;
	float box_w_t = 0.6f; 
    // add small room floor
    obj_list.push_back(new Triangle(Vec3(min_x, min_y, min_z), Vec3(max_x, min_y, min_z), Vec3(min_x, min_y, max_z), box_r_t, box_w_t));
    obj_list.push_back(new Triangle(Vec3(max_x, min_y, min_z), Vec3(max_x, min_y, max_z), Vec3(min_x, min_y, max_z), box_r_t, box_w_t));
    // add small room ceiling
    obj_list.push_back(new Triangle(Vec3(min_x, max_y, max_z), Vec3(max_x, max_y, max_z), Vec3(min_x, max_y, min_z), box_r_t, box_w_t));
    obj_list.push_back(new Triangle(Vec3(max_x, max_y, max_z), Vec3(max_x, max_y, min_z), Vec3(min_x, max_y, min_z), box_r_t, box_w_t));
    // add small room walls on the back
    obj_list.push_back(new Triangle(Vec3(max_x, max_y, max_z), Vec3(min_x, max_y, max_z), Vec3(min_x, min_y, max_z), box_r_t, box_w_t));
    obj_list.push_back(new Triangle(Vec3(min_x, min_y, max_z), Vec3(max_x, min_y, max_z), Vec3(max_x, max_y, max_z), box_r_t, box_w_t));
    // add small room walls on the frontbox_w_t    
	obj_list.push_back(new Triangle(Vec3(min_x, max_y, min_z), Vec3(max_x, max_y, min_z), Vec3(max_x, min_y, min_z), box_r_t, box_w_t));
    obj_list.push_back(new Triangle(Vec3(max_x, min_y, min_z), Vec3(min_x, min_y, min_z), Vec3(min_x, max_y, min_z), box_r_t, box_w_t));
    // add small room walls on the left
    obj_list.push_back(new Triangle(Vec3(min_x, max_y, max_z), Vec3(min_x, max_y, min_z), Vec3(min_x, min_y, min_z), box_r_t, box_w_t));
    obj_list.push_back(new Triangle(Vec3(min_x, min_y, min_z), Vec3(min_x, min_y, max_z), Vec3(min_x, max_y, max_z), box_r_t, box_w_t));
    // add small room walls on the right
    obj_list.push_back(new Triangle(Vec3(max_x, max_y, min_z), Vec3(max_x, max_y, max_z), Vec3(max_x, min_y, max_z), box_r_t, box_w_t));
    obj_list.push_back(new Triangle(Vec3(max_x, min_y, max_z), Vec3(max_x, min_y, min_z), Vec3(max_x, max_y, min_z), box_r_t, box_w_t));
	
	// add main tetrahedron
	Vec3 move(0, 0.5, -3);
	float tetra_r_t = 0.5f;
	float tetra_w_t = 0.6f;
	Vec3 v1 = 0.5 * Vec3( 1,  1,  1) + move;
	Vec3 v2 = 0.5 * Vec3(-1, -1,  1) + move;
	Vec3 v3 = 0.5 * Vec3(-1,  1, -1) + move;
	Vec3 v4 = 1 * Vec3( 1, -1, -1) + move;

	add_tetrahedron(obj_list, v1, v2, v3, v4, tetra_r_t, tetra_w_t);

	// add random spheres
	for (int i = 0; i < 15; i++) {
		float xr = get_random(-2, 2);
		float zr = get_random(-1.5, 1.5);
		float r1 = get_random(0, 1);
		// float r2 = get_random(0, 1) - 0.5;
		// if (r2 < 0){
		// 	r2 = 0;
		// }
		obj_list.push_back(new Sphere(Vec3(xr, -0.45, zr-2), 0.05, 0, 0));
	}

	// add random tetrahedrons
    for (int i = 0; i < 30; i++) {
        float scale = 0.05; 
        
        float max_offset = scale * sqrt(3); 
        
        float tetra_x_min = -4.0f + max_offset;
        float tetra_x_max = 4.0f - max_offset;
        float tetra_y_min = -0.55f + max_offset;
        float tetra_y_max = 4.0f - max_offset;
        float tetra_z_min = -10.0f + max_offset;
        float tetra_z_max = 2.0f - max_offset;
        
        float x = get_random(tetra_x_min, tetra_x_max);
        float y = -0.45; 
        float z = get_random(tetra_z_min, tetra_z_max);

		float r1 = get_random(0, 1);
        float transparency = get_random(0, 0.3f);
        
        Vec3 move(x, y, z);
        Vec3 v1 = scale * Vec3( 1,  1,  1) + move;
        Vec3 v2 = scale * Vec3(-1, -1,  1) + move;
        Vec3 v3 = scale * Vec3(-1,  1, -1) + move;
        Vec3 v4 = scale * Vec3( 1, -1, -1) + move;
        
        add_tetrahedron(obj_list, v1, v2, v3, v4, r1, transparency);
    }

	// add random boxes
    for (int i = 0; i < 10; i++) {
        // Random position 
        float xr = get_random(-2, 2);
        float zr = get_random(-1.5, 1.5);
        
        float cube_size = get_random(0.05f, 0.15f);
        float half_size = cube_size / 2.0f;
        

        float r1 = get_random(0, 1);
        float transparency = get_random(0, 0.3f);
        
        Vec3 center(xr, -0.45f + half_size, zr - 2);
        
        float cube_min_x = center.x() - half_size;
        float cube_max_x = center.x() + half_size;
        float cube_min_y = center.y() - half_size;
        float cube_max_y = center.y() + half_size;
        float cube_min_z = center.z() - half_size;
        float cube_max_z = center.z() + half_size;
        
        // Create cube faces using triangles
        // Bottom face
        obj_list.push_back(new Triangle(Vec3(cube_min_x, cube_min_y, cube_min_z), 
                                        Vec3(cube_max_x, cube_min_y, cube_min_z), 
                                        Vec3(cube_min_x, cube_min_y, cube_max_z), r1, transparency));
        obj_list.push_back(new Triangle(Vec3(cube_max_x, cube_min_y, cube_min_z), 
                                        Vec3(cube_max_x, cube_min_y, cube_max_z), 
                                        Vec3(cube_min_x, cube_min_y, cube_max_z), r1, transparency));
        
        // Top face
        obj_list.push_back(new Triangle(Vec3(cube_min_x, cube_max_y, cube_max_z), 
                                        Vec3(cube_max_x, cube_max_y, cube_max_z), 
                                        Vec3(cube_min_x, cube_max_y, cube_min_z), r1, transparency));
        obj_list.push_back(new Triangle(Vec3(cube_max_x, cube_max_y, cube_max_z), 
                                        Vec3(cube_max_x, cube_max_y, cube_min_z), 
                                        Vec3(cube_min_x, cube_max_y, cube_min_z), r1, transparency));
        
        // Back face
        obj_list.push_back(new Triangle(Vec3(cube_max_x, cube_max_y, cube_max_z), 
                                        Vec3(cube_min_x, cube_max_y, cube_max_z), 
                                        Vec3(cube_min_x, cube_min_y, cube_max_z), r1, transparency));
        obj_list.push_back(new Triangle(Vec3(cube_min_x, cube_min_y, cube_max_z), 
                                        Vec3(cube_max_x, cube_min_y, cube_max_z), 
                                        Vec3(cube_max_x, cube_max_y, cube_max_z), r1, transparency));
        
        // Front face
        obj_list.push_back(new Triangle(Vec3(cube_min_x, cube_max_y, cube_min_z), 
                                        Vec3(cube_max_x, cube_max_y, cube_min_z), 
                                        Vec3(cube_max_x, cube_min_y, cube_min_z), r1, transparency));
        obj_list.push_back(new Triangle(Vec3(cube_max_x, cube_min_y, cube_min_z), 
                                        Vec3(cube_min_x, cube_min_y, cube_min_z), 
                                        Vec3(cube_min_x, cube_max_y, cube_min_z), r1, transparency));
        
        // Left face
        obj_list.push_back(new Triangle(Vec3(cube_min_x, cube_max_y, cube_max_z), 
                                        Vec3(cube_min_x, cube_max_y, cube_min_z), 
                                        Vec3(cube_min_x, cube_min_y, cube_min_z), r1, transparency));
        obj_list.push_back(new Triangle(Vec3(cube_min_x, cube_min_y, cube_min_z), 
                                        Vec3(cube_min_x, cube_min_y, cube_max_z), 
                                        Vec3(cube_min_x, cube_max_y, cube_max_z), r1, transparency));
        
        // Right face
        obj_list.push_back(new Triangle(Vec3(cube_max_x, cube_max_y, cube_min_z), 
                                        Vec3(cube_max_x, cube_max_y, cube_max_z), 
                                        Vec3(cube_max_x, cube_min_y, cube_max_z), r1, transparency));
        obj_list.push_back(new Triangle(Vec3(cube_max_x, cube_min_y, cube_max_z), 
                                        Vec3(cube_max_x, cube_min_y, cube_min_z), 
                                        Vec3(cube_max_x, cube_max_y, cube_min_z), r1, transparency));
    }

}

void create_scene_lights(vector<Light> &lights) {
	for (int i = 0; i < 50; i++) {
		float xr = get_random(-1.5, 1.5);
		float yr = get_random(0, 2);
		float zr = get_random(-6, 1);
	

		float max_intensity = 0.05f;
		float c = get_random(0, max_intensity);
		if(yr < -1.5f + 1 * (3.0f / 6.0f)){
			// pure red
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, 0)));
		}else if(yr < -1.5f + 2 * (3.0f / 6.0f)){
			// red + green
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c, 0)));
		}else if(yr < -1.5f + 3 * (3.0f / 6.0f)){
			// pure green
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, 0)));
		}else if(yr < -1.5f + 4 * (3.0f / 6.0f)){
			// green + blue
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, c)));
		}else if(yr < -1.5f + 5 * (3.0f / 6.0f)){
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
		if(yr < -1.5f + 1 * (3.0f / 6.0f)){
			// pure red
			I = Vec3(c, 0, 0);
		}else if(yr < -1.5f + 2 * (3.0f / 6.0f)){
			// red + green
			I = Vec3(c, c, 0);
		}else if(yr < -1.5f + 3 * (3.0f / 6.0f)){
			// pure green
			I = Vec3(0, c, 0);
		}else if(yr < -1.5f + 4 * (3.0f / 6.0f)){
			// green + blue
			I = Vec3(0, c, c);
		}else if(yr < -1.5f + 5 * (3.0f / 6.0f)){
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