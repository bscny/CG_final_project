#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>

// custom
#include "tools/vec3.h"
#include "tools/ray.h"
#include "tools/object.h"

#include "scene_objects/light.h"
#include "scene_objects/sphere.h"
#include "scene_objects/triangle.h"
#include "scene_objects/mesh.h"

#include "utils/tracer.h"
#include "utils/set_up.h"
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
	// create_scene_objects(obj_list);

	// light params
	vector<Light> lights;
	create_scene_lights(lights);

	fstream file;
	// relative to /build
	if(ANTI_ALIASING){
		file.open("../images/result.ppm", ios::out);
	}else{
		file.open("../images/result_no_anti_aliasing.ppm", ios::out);
	}

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

					c += trace_color_ray(r, BOUNCE, obj_list, lights, -1);
				}

				c /= float(SAMPLES_PER_PIXAL);
			}else{
				float ratio_h = float(i) / float(WIDTH);
				float ratio_v = float(j) / float(HEIGHT);
				Vec3 target_point = lower_left_coner + ratio_h * horizontal + ratio_v * vertical;
	
				Ray r(origin, target_point - origin);
	
				// for each ray, see the final color it contributes to the screen
				c = trace_color_ray(r, BOUNCE, obj_list, lights, -1);
			}

			// clamping
			if(c[0] > 1) c[0] = 1;
			if(c[1] > 1) c[1] = 1;
			if(c[2] > 1) c[2] = 1;

			file << int(c[0] * 255) << " " << int(c[1] * 255) << " " << int(c[2] * 255) << "\n";
		}
	}

	return 0;
}