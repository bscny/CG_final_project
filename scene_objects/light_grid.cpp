#include "light_grid.h"

#include <iostream>

using namespace std;

// the big 3
Node::Node(const Node &in) {
    grid_pos = in.grid_pos;
    intensity = in.intensity;
    wieghted_pos = in.wieghted_pos;
    weight = in.weight;
    max_pos = in.max_pos;
    min_pos = in.min_pos;
    has_light = in.has_light;
    dimension = in.dimension;
    split_value = in.split_value;
    left = in.left;
    right = in.right;
}

Node Node::operator= (const Node &in) {
    grid_pos = in.grid_pos;
    intensity = in.intensity;
    wieghted_pos = in.wieghted_pos;
    weight = in.weight;
    max_pos = in.max_pos;
    min_pos = in.min_pos;
    has_light = in.has_light;
    dimension = in.dimension;
    split_value = in.split_value;
    left = in.left;
    right = in.right;

    return *this;
}

// other constructor
Node::Node(Vec3 max_p, Vec3 min_p, int dim) {
    // leave blank
    grid_pos = Vec3();
    intensity = Vec3();
    wieghted_pos = Vec3();
    weight = 0;

    // give value
    max_pos = max_p;
    min_pos = min_p;
    has_light = false;
    dimension = dim;
    split_value = (min_pos[dim] + max_pos[dim]) / 2;
    left = nullptr;
    right = nullptr;

    // check if input is valid
    if (dim > 2 || dim < 0) {
        exit(-1);
        std::cout << "error dim input when creating node";
    }
}

// getters
Vec3 Node::get_real_light_pos() const {
    if(weight > 0.00000001){
        return wieghted_pos / weight;
    }
    
    return grid_pos;
}

Vec3 Node::get_intensity() const {
    return intensity;
}

Vec3 Node::get_grid_vertex_pos() const {
    return grid_pos;
}

// --------------------------------------------------------------------------------------------------

LightGrid::LightGrid(Vec3 world_max_p, Vec3 world_min_p) {
    world_max_pos = world_max_p;
    world_min_pos = world_min_p;
    size = 0;
    repeation = 0;
    depth = 0;

    // create empty root
    root = new Node(world_max_p, world_min_p, 0);
}

void LightGrid::insert(Vec3 P, Vec3 I, Vec3 W, float weight) {
    insert_recursive(root, P, I, W, weight, 0);
}

void LightGrid::insert_recursive(Node *node, Vec3 P, Vec3 I, Vec3 W, float weight, int current_depth) {
    if (!node->has_light) {
        // If this node doesn't have a point yet, store it here
        // for light info
        node->grid_pos = P;
        node->intensity = I;
        node->wieghted_pos = W;
        node->weight = weight;

        // for kd tree info
        node->has_light = true;
        size ++;

        return;
    } else if (node->grid_pos == P) {
        // the same light in the grid
        node->intensity += I;
        node->wieghted_pos += W;
        node->weight += weight;

        repeation ++;
        return;
    }

    // Node already has a point, so we need to split current sub-space

    // Create children if they don't exist
    // since we will split space, so we will create left and right node at the same time
    if (!node->left) {
        Vec3 left_node_max_pos = node->max_pos;
        Vec3 left_node_min_pos = node->min_pos;
        left_node_max_pos[node->dimension] = node->split_value;
        // create an empty left node
        node->left = new Node(left_node_max_pos, left_node_min_pos, (current_depth + 1) % 3);

        Vec3 right_node_max_pos = node->max_pos;
        Vec3 right_node_min_pos = node->min_pos;
        right_node_min_pos[node->dimension] = node->split_value;
        
        // create an empty right node
        node->right = new Node(right_node_max_pos, right_node_min_pos, (current_depth + 1) % 3);

        if(current_depth + 1 > depth) {
            depth = current_depth + 1;
        }
    }

    // Decide which sub-tree to visit
    if (P[node->dimension] <= node->split_value) {
        insert_recursive(node->left, P, I, W, weight, current_depth + 1);
    } else {
        insert_recursive(node->right, P, I, W, weight, current_depth + 1);
    }
}

void LightGrid::balance() {
    if (size == 0) return;

    // Flatten the current tree into a vector
    vector<Node> nodes;
    flat(nodes, true);
    
    // Create new empty root
    root = new Node(world_max_pos, world_min_pos, 0);

    // reset params
    depth = 0;
    
    // Rebuild tree using median split
    build_balanced_tree(root, nodes, 0, size - 1, 0);
}

void LightGrid::build_balanced_tree(Node* node, vector<Node>& nodes, int start, int end, int current_depth) {
    if (start > end) return;
    
    // Sort by current dimension
    int dim = current_depth % 3;
    sort(nodes.begin() + start, nodes.begin() + end + 1,
            [dim](const Node& a, const Node& b) {
                return a.grid_pos[dim] < b.grid_pos[dim];
            });
    
    // Find median
    int mid = start + (end - start) / 2;
    
    // Set current node data
    node->grid_pos = nodes[mid].grid_pos;
    node->intensity = nodes[mid].intensity;
    node->wieghted_pos = nodes[mid].wieghted_pos;
    node->weight = nodes[mid].weight;
    node->has_light = true;
    
    if (current_depth > depth) {
        depth = current_depth;
    }
    
    // Create and recursively build left subtree
    if (start <= mid - 1) {
        Vec3 left_max = node->max_pos;
        Vec3 left_min = node->min_pos;
        left_max[dim] = node->split_value;
        node->left = new Node(left_max, left_min, (current_depth + 1) % 3);
        build_balanced_tree(node->left, nodes, start, mid - 1, current_depth + 1);
    }
    
    // Create and recursively build right subtree
    if (mid + 1 <= end) {
        Vec3 right_max = node->max_pos;
        Vec3 right_min = node->min_pos;
        right_min[dim] = node->split_value;
        node->right = new Node(right_max, right_min, (current_depth + 1) % 3);
        build_balanced_tree(node->right, nodes, mid + 1, end, current_depth + 1);
    }
}

// Queries
// TODO i dont think we need this api
// Node LightGrid::nearest_neighbor(const Vec3 &target) const {
//     if(size == 0) return;
// }

void LightGrid::range_search(vector<Node> &target_array, Vec3 max_bound, Vec3 min_bound) const {
    // clean the vector first
    target_array.clear();
    
    // AABB test to see if need to recursive
    if(world_min_pos.x() > max_bound.x() || world_max_pos.x() < min_bound.x() ||
       world_min_pos.y() > max_bound.y() || world_max_pos.y() < min_bound.y() ||
       world_min_pos.z() > max_bound.z() || world_max_pos.z() < min_bound.z()) {

        // no intersect
        return;
    }

    range_search_recursive(root, target_array, max_bound, min_bound);
}

void LightGrid::range_search_recursive(Node *node, vector<Node> &target_array, Vec3 max_bound, Vec3 min_bound) const {
    if (!node || !node->has_light) return;
    
    // Check if current point is in range
    if (node->grid_pos.x() >= min_bound.x() && node->grid_pos.x() <= max_bound.x() &&
        node->grid_pos.y() >= min_bound.y() && node->grid_pos.y() <= max_bound.y() &&
        node->grid_pos.z() >= min_bound.z() && node->grid_pos.z() <= max_bound.z()) {

        target_array.push_back(*node);
    }
    
    // Determine which subtrees to search
    if (min_bound[node->dimension] <= node->split_value) {
        range_search_recursive(node->left, target_array, max_bound, min_bound);
    }
    if (max_bound[node->dimension] > node->split_value) {
        range_search_recursive(node->right, target_array, max_bound, min_bound);
    }
}

void LightGrid::radius_search(std::vector<Node> &target_array, Vec3 center, float radius) const {
    // clean the vector first
    target_array.clear();

    radius_search_recursive(root, target_array, center, radius);
}

void LightGrid::radius_search_recursive(Node *node, std::vector<Node> &target_array, Vec3 center, float radius) const {
    if (!node || !node->has_light) return;

    // see if there's intersects between sphere and cube
    // Clamp sphere center to cube bounds
    float closest_x = max(node->min_pos.x(), min(center.x(), node->max_pos.x()) );
    float closest_y = max(node->min_pos.y(), min(center.y(), node->max_pos.y()) );
    float closest_z = max(node->min_pos.z(), min(center.z(), node->max_pos.z()) );

    // Calculate squared distance from sphere center to the closest point on the cube
    float dx = center.x() - closest_x;
    float dy = center.y() - closest_y;
    float dz = center.z() - closest_z;

    float distance_squared = dx*dx + dy*dy + dz*dz;

    if(distance_squared > radius * radius) {
        // no intersect
        return;
    }

    // Check if this node's point is in range
    dx = center.x() - node->grid_pos.x();
    dy = center.y() - node->grid_pos.y();
    dz = center.z() - node->grid_pos.z();

    distance_squared = dx*dx + dy*dy + dz*dz;

    if (distance_squared <= radius * radius) {
        target_array.push_back(*node);
    }
    
    // Search children
    if (node->left) radius_search_recursive(node->left, target_array, center, radius);
    if (node->right) radius_search_recursive(node->right, target_array, center, radius);
}

// transform
void LightGrid::flat(vector<Node> &target_array, bool delete_flag) const {
    target_array.clear();
    inorder(root, target_array, delete_flag);
}

void LightGrid::inorder(Node *node, vector<Node> &target_array, bool delete_flag) const {
    if (node && node->has_light) {
        inorder(node->left, target_array, delete_flag);
        target_array.push_back(*node);
        inorder(node->right, target_array, delete_flag);

        if (delete_flag) delete node;
    }
}

// getters
int LightGrid::get_size() const {
    return size;
}

int LightGrid::get_repeation() const {
    return repeation;
}

int LightGrid::get_depth() const {
    return depth;
}