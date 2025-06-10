#include "insert_lg.h"

using namespace std;

void insert_influenced_grid_vertices(LightGrid &lg, Vec3 light_pos, Vec3 light_intensity, Vec3 grid_org, float h) {
    // Convert light position to grid coordinates
    Vec3 gridCoord = Vec3(
        (light_pos.x() - grid_org.x()) / h,
        (light_pos.y() - grid_org.y()) / h,
        (light_pos.z() - grid_org.z()) / h
    );

    // Find the base grid cell (floor of coordinates)
    int baseX = static_cast<int>(floor(gridCoord.x()));
    int baseY = static_cast<int>(floor(gridCoord.y()));
    int baseZ = static_cast<int>(floor(gridCoord.z()));

    // Calculate fractional parts for trilinear interpolation
    float fx = gridCoord.x() - baseX;
    float fy = gridCoord.y() - baseY;
    float fz = gridCoord.z() - baseZ;

    // The 8 vertices of the grid cell
    int offsets[8][3] = {
        {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
        {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}
    };
    
    // Trilinear weight for each of the 8 vertices
    float trilinear_weight[8] = {
        (1-fx) * (1-fy) * (1-fz),  // 000
        fx     * (1-fy) * (1-fz),  // 100
        (1-fx) * fy     * (1-fz),  // 010
        fx     * fy     * (1-fz),  // 110
        (1-fx) * (1-fy) * fz,      // 001
        fx     * (1-fy) * fz,      // 101
        (1-fx) * fy     * fz,      // 011
        fx     * fy     * fz       // 111
    };

    // Process each of the 8 vertices
    for (int i = 0; i < 8; i++) {
        int vx = baseX + offsets[i][0];
        int vy = baseY + offsets[i][1];
        int vz = baseZ + offsets[i][2];

        float abs_I = sqrt( dot(light_intensity, light_intensity) );

        // check if result is proper (should be)
        if (vx < 0 || vy < 0 || vz < 0) {
            cout << "error on inserting 8 grid vertices, expect greater than 0" << endl;
            exit(-1);
        }

        // Calculate actual world position of grid vertex
        Vec3 grid_vertex_position = Vec3(
            grid_org.x() + vx * h,
            grid_org.y() + vy * h,
            grid_org.z() + vz * h
        );

        // Only include vertices with non-zero trilinear weight
        if (trilinear_weight[i] > 0.0f) {
            lg.insert(grid_vertex_position, trilinear_weight[i] * light_intensity, abs_I * light_pos, abs_I);
        }
    }
}