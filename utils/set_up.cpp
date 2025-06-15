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
	float width = 2.0f;
	float height = 2.0f;
	float depth = 3.0f;

    Vec3 lower_left_corner = camera_position[1];
    Vec3 horizontal = camera_position[2];
    Vec3 vertical = camera_position[3];
	
	// the distance between the plane that is closest to the screen
	float back_box_wall_distence_to_screen = 0.1f;

	Vec3 standard = Vec3(lower_left_corner.x() + horizontal.x() / 2,
						 lower_left_corner.y() + vertical.y() / 2,
						 lower_left_corner.z() - (depth / 2) - back_box_wall_distence_to_screen);

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


   	float small_floor_y = -0.5f;
    float small_ceiling_y = 1.0f;
    float small_left_x = -0.7f;
    float small_right_x = -0.1f;
    float small_back_z = -3.5f;
    float small_front_z = -5.5f;
    
    // add small room floor
    obj_list.push_back(new Triangle(Vec3(small_left_x, small_floor_y, small_front_z), Vec3(small_right_x, small_floor_y, small_front_z), Vec3(small_left_x, small_floor_y, small_back_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(small_right_x, small_floor_y, small_front_z), Vec3(small_right_x, small_floor_y, small_back_z), Vec3(small_left_x, small_floor_y, small_back_z), 0, 0));
    // add small room ceiling
    obj_list.push_back(new Triangle(Vec3(small_left_x, small_ceiling_y, small_back_z), Vec3(small_right_x, small_ceiling_y, small_back_z), Vec3(small_left_x, small_ceiling_y, small_front_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(small_right_x, small_ceiling_y, small_back_z), Vec3(small_right_x, small_ceiling_y, small_front_z), Vec3(small_left_x, small_ceiling_y, small_front_z), 0, 0));

    // add small room walls on the back
    obj_list.push_back(new Triangle(Vec3(small_right_x, small_ceiling_y, small_back_z), Vec3(small_left_x, small_ceiling_y, small_back_z), Vec3(small_left_x, small_floor_y, small_back_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(small_left_x, small_floor_y, small_back_z), Vec3(small_right_x, small_floor_y, small_back_z), Vec3(small_right_x, small_ceiling_y, small_back_z), 0, 0));
    // add small room walls on the front
    obj_list.push_back(new Triangle(Vec3(small_left_x, small_ceiling_y, small_front_z), Vec3(small_right_x, small_ceiling_y, small_front_z), Vec3(small_right_x, small_floor_y, small_front_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(small_right_x, small_floor_y, small_front_z), Vec3(small_left_x, small_floor_y, small_front_z), Vec3(small_left_x, small_ceiling_y, small_front_z), 0, 0));
    // add small room walls on the left
    obj_list.push_back(new Triangle(Vec3(small_left_x, small_ceiling_y, small_back_z), Vec3(small_left_x, small_ceiling_y, small_front_z), Vec3(small_left_x, small_floor_y, small_front_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(small_left_x, small_floor_y, small_front_z), Vec3(small_left_x, small_floor_y, small_back_z), Vec3(small_left_x, small_ceiling_y, small_back_z), 0, 0));
    // add small room walls on the right
    obj_list.push_back(new Triangle(Vec3(small_right_x, small_ceiling_y, small_front_z), Vec3(small_right_x, small_ceiling_y, small_back_z), Vec3(small_right_x, small_floor_y, small_back_z), 0, 0));
    obj_list.push_back(new Triangle(Vec3(small_right_x, small_floor_y, small_back_z), Vec3(small_right_x, small_floor_y, small_front_z), Vec3(small_right_x, small_ceiling_y, small_front_z), 0, 0));

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

// add the the light with the ball cover, its function is to be the light source
// there will be a lot of lights on the air ball face, and spread the lights to 
// light of the scene, and the light in this ball must me the same color light
// , which around over on the AIR ball  
void add_small_ball_with_light(vector<Object *> &obj_list, vector<Light> &lights, vector<Vec3> &bounds){
    float sphere_radius = 0.05f;
	float sphere_pos_offset = 0.1f;
	int num_sphere = 20;
	int num_lights_on_sphere = 6;
	float light_pos_offset = 0.5f * sphere_radius;
	float max_intensity = 0.3f;

    for (int i = 0; i < num_sphere; i++) {
        // Random position within bounds
        float xr = get_random(bounds[1].x() + sphere_pos_offset, bounds[0].x() - sphere_pos_offset);
        float yr = get_random(bounds[1].y() + sphere_pos_offset, bounds[0].y() - sphere_pos_offset);
        float zr = get_random(bounds[1].z() + sphere_pos_offset, bounds[0].z() - sphere_pos_offset);
        
        // Create the sphere object (air ball with refractive index)
        obj_list.push_back(new Sphere(Vec3(xr, yr, zr), sphere_radius, 0, 0, AIR_N));

        // Choose ONE color based on sphere's x-position (same as create_scene_light_grids)
        float x_range = bounds[0].x() - bounds[1].x() - (2 * sphere_pos_offset);
        float x_ratio = (xr - bounds[1].x() - sphere_pos_offset) / x_range;
        
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

		// The 8 vertices of the grid cell
		float directions[num_lights_on_sphere][3] = {
			{1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
			{0, -1, 0}, {0, 0, 1}, {0, 0, -1}
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


            // float light_intensity = get_random(0.05f, 0.3f);
			float light_intensity = get_random(0, max_intensity);
            Vec3 light_color = base_color * light_intensity;

            lights.push_back(Light(Vec3(light_x, light_y, light_z), light_color));
        }
    }
}

// Add glowing spheres with lights that contribute to light grids
// Each sphere acts as a light source with many surface lights of the same color
void add_small_ball_with_lightgrids(vector<Object *> &obj_list, vector<LightGrid> &lgs, vector<Vec3> &bounds){
    float sphere_radius = 0.05f;
	float sphere_pos_offset = 0.1f;
	int num_sphere = 20;
	int num_lights_on_sphere = 6;
	float light_pos_offset = 0.5f * sphere_radius;
	float max_intensity = 0.3f;

	int lv_num = 3;
	for (int i = 0; i <= lv_num; i ++) {
		// top right, bottom left
		lgs.push_back(LightGrid(Vec3(bounds[0].x(), bounds[0].y(), bounds[0].z()), 
								Vec3(bounds[1].x(), bounds[1].y(), bounds[1].z())));
	}

    for (int i = 0; i < num_sphere; i++) {
        // Random position within bounds
        float xr = get_random(bounds[1].x() + sphere_pos_offset, bounds[0].x() - sphere_pos_offset);
        float yr = get_random(bounds[1].y() + sphere_pos_offset, bounds[0].y() - sphere_pos_offset);
        float zr = get_random(bounds[1].z() + sphere_pos_offset, bounds[0].z() - sphere_pos_offset);
        
        // Create the sphere object (air ball with refractive index)
        obj_list.push_back(new Sphere(Vec3(xr, yr, zr), sphere_radius, 0, 0, AIR_N));

        // Choose ONE color based on sphere's x-position (same as create_scene_light_grids)
        float x_range = bounds[0].x() - bounds[1].x() - (2 * sphere_pos_offset);
        float x_ratio = (xr - bounds[1].x() - sphere_pos_offset) / x_range;

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

		// The 8 vertices of the grid cell
		float directions[num_lights_on_sphere][3] = {
			{1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
			{0, -1, 0}, {0, 0, 1}, {0, 0, -1}
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
				insert_influenced_grid_vertices(lgs[i], Vec3(light_x, light_y, light_z), light_color, bounds[1], H * pow(2, i));
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

void create_scene_objects(vector<Object *> &obj_list, vector<Vec3> &bounds) {
	// add main sphere
	// obj_list.push_back(new Sphere(Vec3(0, 0, -2), 0.5, 0, 0, GLASS_N));
	// obj_list.push_back(new Sphere(Vec3(1, 0, -1.75), 0.5, 0.6, 0, WATER_N));
	obj_list.push_back(new Sphere(Vec3(-1.5, 0, -3), 0.5, 0.8, 0, DIAMOND_N));

	// add tetrahedron
	Vec3 move(2, 0.5, -4.1);
	Vec3 v1 = 0.5 * Vec3( 1,  1,  1) + move;
	Vec3 v2 = 0.5 * Vec3(-1, -1,  1) + move;
	Vec3 v3 = 0.5 * Vec3(-1,  1, -1) + move;
	Vec3 v4 = 1 * Vec3( 1, -1, -1) + move;

	add_tetrahedron(obj_list, v1, v2, v3, v4, 0, 0);

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

void create_scene_lights(vector<Light> &lights, vector<Vec3> &bounds) {
    for (int i = 0; i < 1; i++) {
		float xr = get_random(-1.5, 1.5);
		float zr = get_random(-6, 1);
		float yr = get_random(0, 2);
        
        float max_intensity = 2.0f;
        float c = max_intensity; 
        lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c, c)));
    //     if(i < 1){
    //         // pure red 
    //         lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, 0)));
    //     }else if(i < 2){
    //         // red + green 
    //         lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c*0.5f, 0)));
    //     }else if(i < 3){
    //         // yellow
    //         lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, c, 0)));
    //     }else if(i < 5){
    //         // pure green
    //         lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, 0)));
    //     }else if(i < 7){
    //         // green + blue 
    //         lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, c, c)));
    //     }else if(i < 9){
    //         // pure blue
    //         lights.push_back(Light(Vec3(xr, yr, zr), Vec3(0, 0, c)));
    //     }else{
    //         // red + blue 
    //         lights.push_back(Light(Vec3(xr, yr, zr), Vec3(c, 0, c)));
    //     }
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
	vector<Vec3> bounds = create_bounded_box(camera_position);
	create_scene_objects(obj_list, bounds);
	add_small_ball_with_lightgrids(obj_list, lgs, bounds);
	// create_scene_light_grids(lgs, bounds);
}

// for light
void create_scene(vector<Object *> &obj_list, vector<Vec3> &camera_position, vector<Light> &lights) {
	vector<Vec3> bounds = create_bounded_box(camera_position);
	create_scene_objects(obj_list, bounds);
	add_box(obj_list);
	// add_small_ball_with_light(obj_list, lights, bounds);
	create_scene_lights(lights, bounds);
}