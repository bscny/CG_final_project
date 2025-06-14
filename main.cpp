#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <random>
// custom
#include "tools/vec3.h"
#include "tools/ray.h"
#include "tools/object.h"

#include "scene_objects/light.h"
#include "scene_objects/light_grid.h"
#include "scene_objects/sphere.h"
#include "scene_objects/triangle.h"
#include "scene_objects/mesh.h"

#include "utils/tracer.h"
#include "utils/set_up.h"

#include "constant.h"

using namespace std;

void render_chunk(int start_row, int end_row,
                 const Vec3& lower_left_corner, const Vec3& origin,
                 const Vec3& horizontal, const Vec3& vertical,
                 const vector<Object*>& obj_list, const vector<LightGrid>& lgs,
                 vector<vector<Vec3>>& image_buffer) {
    
    // Thread-local random number generator for anti-aliasing
    thread_local random_device rd;
    thread_local mt19937 gen(rd());
    thread_local uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    for (int j = start_row; j < end_row; j++) {
        for (int i = 0; i < WIDTH; i++) {
            Vec3 c(0, 0, 0);
            
            if (ANTI_ALIASING) {
                // Take multiple samples per pixel for anti-aliasing
                for (int s = 0; s < SAMPLES_PER_PIXAL; s++) {
                    float u = ((float)i + dis(gen)) / float(WIDTH);
                    float v = ((float)j + dis(gen)) / float(HEIGHT);
                    Vec3 target_point = lower_left_corner + u * horizontal + v * vertical;
                    
                    Ray r(origin, target_point - origin);
                    c += trace_color_ray_lg(r, BOUNCE, obj_list, lgs, -1);
                }
                c /= float(SAMPLES_PER_PIXAL);
            } else {
                // Single sample per pixel (no anti-aliasing)
                float ratio_h = float(i) / float(WIDTH);
                float ratio_v = float(j) / float(HEIGHT);
                Vec3 target_point = lower_left_corner + ratio_h * horizontal + ratio_v * vertical;
                
                Ray r(origin, target_point - origin);
                c = trace_color_ray_lg(r, BOUNCE, obj_list, lgs, -1);
            }
            
            // Clamp colors
            if(c[0] > 1) c[0] = 1;
            if(c[1] > 1) c[1] = 1;
            if(c[2] > 1) c[2] = 1;
            
            image_buffer[j][i] = c;
        }
    }
}

int main() {
    // current coordinates: cam looking down in -z direction
	//
	//       Z
	//       |            
	//       |              
	//       |_________ Y
	//      /
	//     /
	//    X
	//

	// in cam: we have xy coordinates of this
	//                  -z
	//					/
	//                 /
	//		 Y
	//		 |
	//		 |
	//		 |_________ X
	//
    
    // Camera params
    Vec3 lower_left_corner(-2, -1, -1);
    Vec3 origin(0, 0, 1);
    Vec3 horizontal(4, 0, 0);
    Vec3 vertical(0, 2, 0);
    
    vector<Vec3> camera_position = {origin, lower_left_corner, horizontal, vertical};
    // Scene object params
    vector<Object*> obj_list;
    
    // Light params
    vector<LightGrid> lgs;
    create_scene(obj_list, camera_position, lgs);
    
    // Image buffer to store results
    vector<vector<Vec3>> image_buffer(HEIGHT, vector<Vec3>(WIDTH));
    
    // Get number of hardware threads
    unsigned int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4; // fallback
    
    cout << "Using " << num_threads << " threads for rendering..." << endl;
    
    // Chunk-based threading
    vector<thread> threads;
    int rows_per_thread = HEIGHT / num_threads;
    int remaining_rows = HEIGHT % num_threads;
    
    for (unsigned int t = 0; t < num_threads; t++) {
        int start_row = t * rows_per_thread;
        int end_row = start_row + rows_per_thread;
        
        // Distribute remaining rows to first few threads
        if ((int)t < remaining_rows) {
            start_row += t;
            end_row += t + 1;
        } else {
            start_row += remaining_rows;
            end_row += remaining_rows;
        }
        
        threads.emplace_back(render_chunk, start_row, end_row,
                           lower_left_corner, origin, horizontal, vertical,
                           cref(obj_list), cref(lgs), ref(image_buffer));
    }
    
    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }
    
    // Write results to file
    fstream file;
    if (ANTI_ALIASING) {
        file.open("../images/light_grid.ppm", ios::out);
    } else {
        file.open("../images/light_grid_no_anti_aliasing.ppm", ios::out);
    }
    
    file << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
    
    // Write image buffer to file
    for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            Vec3 c = image_buffer[j][i];
            file << int(c[0] * 255) << " " << int(c[1] * 255) << " " << int(c[2] * 255) << "\n";
        }
    }
    
    return 0;
}