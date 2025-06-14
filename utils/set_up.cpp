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

vector<Vec3> create_bounded_box(vector<Vec3> &camera_position){
	float distance_to_screen = -1.0f;
	float width = 8.0f;
	float height = 4.5f;
	float depth = 12.0f;
         
    Vec3 lower_left_corner = camera_position[1]; 
    Vec3 horizontal = camera_position[2];        
    Vec3 vertical = camera_position[3];          

	Vec3 standard = Vec3(lower_left_corner.x() + horizontal.x() / 2, 
						 lower_left_corner.y() + vertical.y() / 2, 
						 lower_left_corner.z() + distance_to_screen);

	// top right, bottom left
	Vec3 top_right = Vec3(standard.x() + width / 2, 
						  standard.y() + height / 2, 
						  standard.z() + depth / 2);

	Vec3 bottom_left = Vec3(standard.x() - width / 2,
						    standard.y() - height / 2, 
						    standard.z() - depth / 2);

	vector<Vec3> bounds = {top_right, bottom_left};

	return bounds;
}

void add_box(vector<Object *> &obj_list, vector<Vec3> &bounds) {
	float maxlength = 100.0f;
	// Vec3 center = Vec3(bounds[0].x() + bounds[1].x(), 
	// 				   bounds[0].y() + bounds[1].y(), 
	// 				   bounds[0].z());

	// add the scene container
	// add floor
	obj_list.push_back(new Triangle(Vec3(-maxlength, bounds[1].y(), maxlength), Vec3(maxlength, bounds[1].y(), maxlength), Vec3(-maxlength, bounds[1].y(), -maxlength), 0, 0));
	obj_list.push_back(new Triangle(Vec3(maxlength, bounds[1].y(), maxlength), Vec3(maxlength, bounds[1].y(), -maxlength), Vec3(-maxlength, bounds[1].y(), -maxlength), 0, 0));
	// add ceiling
	obj_list.push_back(new Triangle(Vec3(-maxlength, bounds[0].y(), -maxlength), Vec3(maxlength, bounds[0].y(), -maxlength), Vec3(-maxlength, bounds[0].y(), maxlength), 0, 0));
	obj_list.push_back(new Triangle(Vec3(maxlength, bounds[0].y(), -maxlength), Vec3(maxlength, bounds[0].y(), maxlength), Vec3(-maxlength, bounds[0].y(), maxlength), 0, 0));

	// add walls on the back
	obj_list.push_back(new Triangle(Vec3(maxlength, maxlength, bounds[1].z()), Vec3(-maxlength, maxlength, bounds[1].z()), Vec3(-maxlength, -maxlength, bounds[1].z()), 0, 0));
	obj_list.push_back(new Triangle(Vec3(-maxlength, -maxlength, bounds[1].z()), Vec3(maxlength, -maxlength, bounds[1].z()), Vec3(maxlength, maxlength, bounds[1].z()), 0, 0));
	// add walls on the front
	obj_list.push_back(new Triangle(Vec3(-maxlength, maxlength, bounds[0].z() +100), Vec3(maxlength, maxlength, bounds[0].z()+100), Vec3(maxlength, -maxlength, bounds[0].z()+100), 0, 0));
	obj_list.push_back(new Triangle(Vec3(maxlength, -maxlength, bounds[0].z() +100), Vec3(-maxlength, -maxlength, bounds[0].z()+100), Vec3(-maxlength, maxlength, bounds[0].z()+100), 0, 0));
	
	// add walls on the left
	obj_list.push_back(new Triangle(Vec3(bounds[1].x(), maxlength, -maxlength), Vec3(bounds[1].x(), maxlength, maxlength), Vec3(bounds[1].x(), -maxlength, maxlength), 0, 0));
	obj_list.push_back(new Triangle(Vec3(bounds[1].x(), -maxlength, maxlength), Vec3(bounds[1].x(), -maxlength, -maxlength), Vec3(bounds[1].x(), maxlength, -maxlength), 0, 0));
	// add walls on the right
	obj_list.push_back(new Triangle(Vec3(bounds[0].x(), maxlength, maxlength), Vec3(bounds[0].x(), maxlength, -maxlength), Vec3(bounds[0].x(), -maxlength, -maxlength), 0, 0));
	obj_list.push_back(new Triangle(Vec3(bounds[0].x(), -maxlength, -maxlength), Vec3(bounds[0].x(), -maxlength, maxlength), Vec3(bounds[0].x(), maxlength, maxlength), 0, 0));


	// add random small boxes
	// ToDo: face have some problems
	// Mr.Yu give me some hints please!
	// for (int i = 0; i < 10; i++) {
	// 	float length = 0.5f;
	// 	// bottom left corner
	// 	float xr = get_random(bounds[1].x() + 1, bounds[0].x() - 1);
	// 	float yr = get_random(bounds[1].y() + 1, bounds[0].y() - 1);
	// 	float zr = get_random(bounds[1].z() + 1, bounds[0].z() - 1);
	// 	// top right corner
	// 	float xr2 = xr + length;
	// 	float yr2 = yr + length;
	// 	float zr2 = zr + length;

	// 	// bottom face
	// 	obj_list.push_back(new Triangle(Vec3(xr, yr, zr), Vec3(xr2, yr, zr2), Vec3(xr, yr, zr), 0, 0));
	// 	obj_list.push_back(new Triangle(Vec3(xr2, yr, zr2), Vec3(xr2, yr, zr), Vec3(xr, yr, zr), 0, 0));
	// 	// top face
	// 	obj_list.push_back(new Triangle(Vec3(xr, yr2, zr), Vec3(xr2, yr2, zr), Vec3(xr, yr2, zr2), 0, 0));
	// 	obj_list.push_back(new Triangle(Vec3(xr2, yr2, zr), Vec3(xr2, yr2, zr2), Vec3(xr, yr2, zr2), 0, 0));
	// 	// back face
	// 	obj_list.push_back(new Triangle(Vec3(xr2, yr2, zr), Vec3(xr, yr2, zr), Vec3(xr, yr, zr), 0, 0));
	// 	obj_list.push_back(new Triangle(Vec3(xr, yr, zr), Vec3(xr2, yr, zr), Vec3(xr2, yr2, zr), 0, 0));
	// 	// front face
	// 	obj_list.push_back(new Triangle(Vec3(xr, yr2, zr2), Vec3(xr2, yr2, zr2), Vec3(xr2, yr, zr2), 0, 0));
	// 	obj_list.push_back(new Triangle(Vec3(xr2, yr, zr2), Vec3(xr, yr, zr2), Vec3(xr, yr2, zr2), 0, 0));
	// 	// left face
	// 	obj_list.push_back(new Triangle(Vec3(xr, yr2, zr), Vec3(xr, yr2, zr2), Vec3(xr, yr, zr2), 0, 0));
	// 	obj_list.push_back(new Triangle(Vec3(xr, yr, zr2), Vec3(xr, yr, zr), Vec3(xr, yr2, zr), 0, 0));
	// 	// right face
	// 	obj_list.push_back(new Triangle(Vec3(xr2, yr2, zr2), Vec3(xr2, yr2, zr), Vec3(xr2, yr, zr), 0, 0));
	// 	obj_list.push_back(new Triangle(Vec3(xr2, yr, zr), Vec3(xr2, yr, zr2), Vec3(xr2, yr2, zr2), 0, 0));
	// }

}

// add the the light with the ball cover, its function is to be the light source
// there will be a lot of lights on the air ball face, and spread the lights to 
// light of the scene, and the light in this ball must me the same color light
// , which around over on the AIR ball  
void add_small_ball_with_light(vector<Object *> &obj_list, vector<Light> &lights, vector<Vec3> &bounds){
    float sphere_radius = 0.05f;
	int num_sphere = 10;
	int num_lights = 50;
	float light_intensity = 0.005f;
    for (int i = 0; i < num_sphere; i++) {
        // Random position within bounds
        float xr = get_random(bounds[0].x() -0.1f, bounds[1].x() + 0.1f);
        float yr = get_random(bounds[0].y() -0.1f, bounds[1].y() + 0.1f);
        float zr = get_random(bounds[0].z() -0.1f, bounds[1].z() + 0.1f);
        
        // Create the sphere object (air ball with refractive index)
        obj_list.push_back(new Sphere(Vec3(xr, yr, zr), sphere_radius, 0, 0, AIR_N));
        
        // Number of lights inside this sphere
        // int num_lights = (int)get_random(20, 50);

        // Choose ONE color based on sphere's x-position (like create_scene_lights)
        float x_range = bounds[1].x() - bounds[0].x();  
        float x_ratio = (xr - bounds[0].x()) / x_range;  
        
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

        // Distribute lights uniformly on the sphere surface
        // All lights on this sphere will have the same color but different intensities
        for (int j = 0; j < num_lights; j++) {
            // Use uniform distribution on sphere surface
            float theta = get_random(0, 2 * M_PI);  // Azimuthal angle
            float phi = acos(get_random(-1, 1));    // Polar angle (uniform on sphere surface)
            
            // Place lights exactly on the sphere surface
            float light_offset = sphere_radius + 0.01f;  // Small gap to prevent noise
            float light_x = xr + light_offset * sin(phi) * cos(theta);
            float light_y = yr + light_offset * sin(phi) * sin(theta);
            float light_z = zr + light_offset * cos(phi);
            
            
            // float light_intensity = get_random(0.05f, 0.3f);
            Vec3 light_color = base_color * light_intensity;
            
            lights.push_back(Light(Vec3(light_x, light_y, light_z), light_color));
        }
    }
}

// Add glowing spheres with lights that contribute to light grids
// Each sphere acts as a light source with many surface lights of the same color
void add_small_ball_with_lightgrids(vector<Object *> &obj_list, vector<LightGrid> &lgs, vector<Vec3> &bounds){
    float sphere_radius = 0.05f;
	int num_sphere = 10;
	int num_lights = 50;
	// int num_lights = (int)get_random(20, 50);
	float light_intensity = 15.0f;
	// float light_intensity = get_random(0.05f, 0.5f) * max_intensity;

	int lv_num = 3;
	for (int i = 0; i <= lv_num; i ++) {
		// top right, bottom left
		lgs.push_back(LightGrid(Vec3(bounds[0].x(), bounds[0].y(), bounds[0].z()), 
								Vec3(bounds[1].x(), bounds[1].y(), bounds[1].z())));
	}

    for (int i = 0; i < num_sphere; i++) {
        // Random position within bounds
        float xr = get_random(bounds[0].x()-0.1f, bounds[1].x() + 0.1f);
        float yr = get_random(bounds[0].y()-0.1f, bounds[1].y() + 0.1f);
        float zr = get_random(bounds[0].z()-0.1f, bounds[1].z() + 0.1f);
        
     
        // Create the sphere object (air ball with refractive index)
        obj_list.push_back(new Sphere(Vec3(xr, yr, zr), sphere_radius, 0, 0, AIR_N));
        
        
        // Choose ONE color based on sphere's x-position (same as create_scene_light_grids)
        float x_range = bounds[1].x() - bounds[0].x();  
        float x_ratio = (xr - bounds[0].x()) / x_range;  
        
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


        for (int j = 0; j < num_lights; j++) {
            float theta = get_random(0, 2 * M_PI);  // Azimuthal angle
            float phi = acos(get_random(-1, 1));    // Polar angle (uniform on sphere surface)
            
            // Place lights slightly outside the sphere surface to avoid numerical issues
            float light_offset = sphere_radius + 0.02f;  // Small gap to prevent noise
            float light_x = xr + light_offset * sin(phi) * cos(theta);
            float light_y = yr + light_offset * sin(phi) * sin(theta);
            float light_z = zr + light_offset * cos(phi);
            
            Vec3 light_color = base_color * light_intensity;
            
			// get the influenced grid vertices for this light at each lv
			for (int i = 0; i <= lv_num; i ++) {
				insert_influenced_grid_vertices(lgs[i], Vec3(light_x, light_y, light_z), light_color, bounds[1], H * pow(2, i));
			} 
        }
    }
}

void create_scene_objects(vector<Object *> &obj_list, vector<Vec3> &bounds){

	// add main sphere
    float center_x = (bounds[0].x() + bounds[1].x()) / 2;
    float center_y = (bounds[0].y() + bounds[1].y()) / 2;
    float center_z = (bounds[0].z() + bounds[1].z()) / 2;
    
    // add main spheres 
	obj_list.push_back(new Sphere(Vec3(center_x - 1.2, center_y, center_z), 0.5, 0.8, 0, DIAMOND_N));
    obj_list.push_back(new Sphere(Vec3(center_x, center_y, center_z), 0.5, 0, 0, GLASS_N));
    obj_list.push_back(new Sphere(Vec3(center_x + 1.2, center_y, center_z), 0.5, 0.6, 0, WATER_N));
    
   
	// add tetrahedron
	/*Vec3 move(0, 0, -3);
	Vec3 v1 = 1 * Vec3( 1,  1,  1) + move;
	Vec3 v2 = 1 * Vec3(-1, -1,  1) + move;
	Vec3 v3 = 1 * Vec3(-1,  1, -1) + move;
	Vec3 v4 = 1 * Vec3( 1, -1, -1) + move;

	add_tetrahedron(obj_list, v1, v2, v3, v4, 0, 0);*/
	add_box(obj_list, bounds);
	// add random objs
	// for (int i = 0; i < 40; i++) {
	// 	float xr = get_random(bounds[1].x(), bounds[0].x());
	// 	float yr = get_random(bounds[1].y(), bounds[0].y());
	// 	float zr = get_random(bounds[1].z(), bounds[0].z());
	// 	float r1 = get_random(-1, 1);
	// 	// float r2 = get_random(0, 1) - 0.5;
	// 	// if (r2 < 0){
	// 	// 	r2 = 0;
	// 	// }
	// 	obj_list.push_back(new Sphere(Vec3(xr, yr, zr-2), 0.05, r1, 0));
	// }
}


void create_scene_lights(vector<Light> &lights, vector<Vec3> &bounds) {
	for (int i = 0; i < 1; i++) {
		float xr = get_random(bounds[1].x(), bounds[0].x());
		float yr = get_random(bounds[1].y(), bounds[0].y());
		float zr = get_random(bounds[1].z(), bounds[0].z());

		float max_intensity = 0.5f;
		float c = get_random(0, max_intensity);
		float x_range = bounds[0].x() - bounds[1].x();  
		float x_ratio = (xr - bounds[1].x()) / x_range;  

		if(x_ratio < 1.0f / 6.0f){
			// pure red
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, 0)));
		}else if(x_ratio < 2.0f / 6.0f){
			// red + green
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c, 0)));
		}else if(x_ratio < 3.0f / 6.0f){
			// pure green
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, 0)));
		}else if(x_ratio < 4.0f / 6.0f){
			// green + blue
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, c)));
		}else if(x_ratio < 5.0f / 6.0f){
			// pure blue
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, 0, c)));
		}else{
			// red + blue
			lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, c)));
		}
	}
}

void create_scene_light_grids(vector<LightGrid> &lgs, vector<Vec3> &bounds) {
	lgs.clear();

	// create all levels of lg
	int lv_num = 3;
	for (int i = 0; i <= lv_num; i ++) {
		// top right, bottom left
		lgs.push_back(LightGrid(Vec3(bounds[0].x(), bounds[0].y(), bounds[0].z()), 
								Vec3(bounds[1].x(), bounds[1].y(), bounds[1].z())));
	}

	// creating VPL
	for (int i = 0; i < 50; i++) {
		float xr = get_random(bounds[1].x(), bounds[0].x());
		float yr = get_random(bounds[1].y(), bounds[0].y());
		float zr = get_random(bounds[1].z(), bounds[0].z());
		Vec3 I;

		float max_intensity = 25.0f;
		float c = get_random(0, max_intensity);

		float x_range = bounds[0].x() - bounds[1].x();
		float x_ratio = (xr - bounds[1].x()) / x_range;

		if(x_ratio < 1.0f / 6.0f){
			// pure red
			I = Vec3(c, 0, 0);
		}else if(x_ratio < 2.0f / 6.0f){
			// red + green
			I = Vec3(c, c, 0);
		}else if(x_ratio < 3.0f / 6.0f){
			// pure green
			I = Vec3(0, c, 0);
		}else if(x_ratio < 4.0f / 6.0f){
			// green + blue
			I = Vec3(0, c, c);
		}else if(x_ratio < 5.0f / 6.0f){
			// pure blue
			I = Vec3(0, 0, c);
		}else{
			// red + blue
			I = Vec3(c, 0, c);
		}

		// get the influenced grid vertices for this light at each lv
		for (int i = 0; i <= lv_num; i ++) {
			insert_influenced_grid_vertices(lgs[i], Vec3(xr, yr, zr), I, bounds[1], H * pow(2, i));
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
	vector<Vec3> bounds = create_bounded_box(camera_position);
	create_scene_objects(obj_list, bounds);
	add_small_ball_with_lightgrids(obj_list, lgs, bounds);
	// create_scene_light_grids(lgs, bounds);
}

// for light
void create_scene(vector<Object *> &obj_list, vector<Vec3> &camera_position, vector<Light> &lights) {
	vector<Vec3> bounds = create_bounded_box(camera_position);
	create_scene_objects(obj_list, bounds);
	add_small_ball_with_light(obj_list, lights, bounds);
	// create_scene_lights(lights, bounds);
}