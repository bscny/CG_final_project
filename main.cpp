#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>

// custom
#include "tools/vec3.h"
#include "tools/ray.h"
#include "tools/object.h"

#include "scene_objects/sphere.h"
#include "scene_objects/triangle.h"
#include "scene_objects/mesh.h"

#include "utils/tracer.h"
#include "constant.h"

using namespace std;

int main()
{
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

	// camera params
	Vec3 lower_left_coner(-2, -1, -1);
	Vec3 origin(0, 0, 1);
	Vec3 horizontal(4, 0, 0);
	Vec3 vertical(0, 2, 0);


	// scene object params
	vector<Object *> obj_list;

	// add floor
	// obj_list.push_back(new Sphere(Vec3(0, -100.5, -2), 100));
	obj_list.push_back(new Triangle(Vec3(-100, -0.55, 100), Vec3(100, -0.55, 100), Vec3(-100, -0.55, -100)));
	obj_list.push_back(new Triangle(Vec3(100, -0.55, 100), Vec3(100, -0.55, -100), Vec3(-100, -0.55, -100)));

	// add main sphere
	obj_list.push_back(new Sphere(Vec3(0, 0, -2), 0.5, 0, 0.9));
	obj_list.push_back(new Sphere(Vec3(1, 0, -1.75), 0.5, 1));
	obj_list.push_back(new Sphere(Vec3(-1, 0, -2.25), 0.5));

	// add tetrahedron
	/*Vec3 move(0, 0, -3);
	Vec3 v1 = 1 * Vec3( 1,  1,  1) + move;
	Vec3 v2 = 1 * Vec3(-1, -1,  1) + move;
	Vec3 v3 = 1 * Vec3(-1,  1, -1) + move;
	Vec3 v4 = 1 * Vec3( 1, -1, -1) + move;

	add_tetrahedron(obj_list, v1, v2, v3, v4, 0, 0);*/

	// add random objs
	srand(12345);
	for (int i = 0; i < 48; i++) {
		float xr = ((float)rand() / (float)(RAND_MAX)) * 6.0f - 3.0f;
		float zr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;
		float r1 = ((float)rand() / (float)(RAND_MAX));
		float r2 = ((float)rand() / (float)(RAND_MAX)) - 0.5;
		if (r2 < 0){
			r2 = 0;
		}
		obj_list.push_back(new Sphere(Vec3(xr, -0.45, zr-2), 0.05, r1, r2));
	}

	// light params
	vector<Vec3> light_sources;
	vector<Vec3> light_intensities;

	// set up lights
	light_sources.push_back(Vec3(-10, 10, 0));
	// light_intensities.push_back(Vec3(1, 1, 1));
	light_intensities.push_back(Vec3(0.5, 0, 0));
	light_sources.push_back(Vec3(10, 10, 0));
	light_intensities.push_back(Vec3(0.25, 0.25, 0.5));
	light_sources.push_back(Vec3(0, 10, 10));
	light_intensities.push_back(Vec3(0, 0.5, 0));
	light_sources.push_back(Vec3(0, 10, -10));
	light_intensities.push_back(Vec3(0.25, 0.25, 0.5));

	fstream file;
	// relative to /build
	file.open("../images/result.ppm", ios::out);

	file << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
	for (int j = HEIGHT - 1; j >= 0; j--) {
		for (int i = 0; i < WIDTH; i++) {
			Vec3 c(0, 0, 0);

			if(ANTI_ALIASING){
				for (int s = 0; s < SAMPLES_PER_PIXAL; s++) {
					float u = ((float)i + static_cast<float>(rand()) / RAND_MAX) / WIDTH;
					float v = ((float)j + static_cast<float>(rand()) / RAND_MAX) / HEIGHT;

					Vec3 target_point = lower_left_coner + u * horizontal + v * vertical;
					Ray r(origin, target_point - origin);

					c += color(r, 5, obj_list, light_sources, light_intensities);
				}

				c /= float(SAMPLES_PER_PIXAL);
			}else{
				float ratio_h = float(i) / float(WIDTH);
				float ratio_v = float(j) / float(HEIGHT);
				Vec3 target_point = lower_left_coner + ratio_h * horizontal + ratio_v * vertical;
	
				Ray r(origin, target_point - origin);
	
				// for each ray, see the final color it contributes to the screen
				c = color(r, 5, obj_list, light_sources, light_intensities);
			}

			file << int(c[0] * 255) << " " << int(c[1] * 255) << " " << int(c[2] * 255) << "\n";
		}
	}

	return 0;
}