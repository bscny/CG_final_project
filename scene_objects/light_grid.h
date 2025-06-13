#ifndef LIGHT_GRID_H
#define LIGHT_GRID_H

#include <vector>
#include <algorithm>
#include <cmath>

#include "tools/vec3.h"
#include "scene_objects/light.h"

struct Node {
    // needed for the light grid
    Vec3 grid_pos;
    Vec3 intensity;
    Vec3 wieghted_pos;
    float weight;

    // needed for the kd tree
    Vec3 max_pos;
    Vec3 min_pos;
    bool has_light;
    int dimension;  // Split dimension (0=x, 1=y, 2=z)
    float split_value;
    Node* left;
    Node* right;

    // the big three
    // Node(); no empty node allowed
    Node(const Node &in);
    Node operator= (const Node &in);

    // constructor
    Node(Vec3 max_p, Vec3 min_p, int dim);

    // getter
    Vec3 get_real_light_pos() const;
    Vec3 get_intensity() const;
    Vec3 get_grid_vertex_pos() const;
};

class LightGrid {
    public:
        LightGrid(Vec3 world_max_p, Vec3 world_min_p);

        void insert(Vec3 P, Vec3 I, Vec3 W, float weight);
        void balance();

        // queries
        // Node nearest_neighbor(const Vec3 &target) const;  // TODO i dont think we need this api
        void range_search(std::vector<Node> &target_array, Vec3 max_bound, Vec3 min_bound) const;
        void radius_search(std::vector<Node> &target_array, Vec3 center, float radius) const;

        // transform to a vector
        void flat(std::vector<Node> &target_array) const;

        // getters
        int get_size() const;
        int get_repeation() const;
        int get_depth() const;
    private:
        Node *root;
        int size;
        int repeation;
        int depth;
        Vec3 world_max_pos;
        Vec3 world_min_pos;

        // recursive helper function
        void insert_recursive(Node *node, Vec3 P, Vec3 I, Vec3 W, float weight, int current_depth);
        void build_balanced_tree(Node* node, std::vector<Node>& nodes, int start, int end, int current_depth);
        void range_search_recursive(Node *node, std::vector<Node> &target_array, Vec3 max_bound, Vec3 min_bound) const;
        void radius_search_recursive(Node *node, std::vector<Node> &target_array, Vec3 center, float radius) const;
        void inorder(Node *node, std::vector<Node> &target_array) const;
};

#endif