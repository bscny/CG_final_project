#include "./kd_tree_median_split.h"

// Point3D implementation
template<typename T>
Point3D<T>::Point3D() : x(0), y(0), z(0) {}

template<typename T>
Point3D<T>::Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

template<typename T>
T& Point3D<T>::operator[](int dim) {
    return (dim == 0) ? x : (dim == 1) ? y : z;
}

template<typename T>
const T& Point3D<T>::operator[](int dim) const {
    return (dim == 0) ? x : (dim == 1) ? y : z;
}

// Node implementation
template<typename T>
KDTree3D<T>::Node::Node(const Point3D<T>& p, int dim) 
    : point(p), dimension(dim), left(nullptr), right(nullptr) {}

template<typename T>
KDTree3D<T>::Node::~Node() {
    delete left;
    delete right;
}

// DimensionComparator implementation
template<typename T>
KDTree3D<T>::DimensionComparator::DimensionComparator(int d) : dim(d) {}

template<typename T>
bool KDTree3D<T>::DimensionComparator::operator()(const Point3D<T>& a, const Point3D<T>& b) {
    return a[dim] < b[dim];
}

// KDTree3D implementation
template<typename T>
KDTree3D<T>::KDTree3D() : root(nullptr) {}

template<typename T>
KDTree3D<T>::KDTree3D(const Point3D<T>& minBounds_, const Point3D<T>& maxBounds_) 
    : root(nullptr), minBounds(minBounds_), maxBounds(maxBounds_) {}

template<typename T>
KDTree3D<T>::~KDTree3D() {
    delete root;
}

template<typename T>
typename KDTree3D<T>::Node* KDTree3D<T>::buildTree(std::vector<Point3D<T>>& points, int depth, int start, int end) {
    if (start > end) return nullptr;
    
    int dim = depth % 3;  // cycle through x, y, z
    
    // Sort points by current dimension
    std::sort(points.begin() + start, points.begin() + end + 1, DimensionComparator(dim));
    
    // Find median
    int median = start + (end - start) / 2;
    
    Node* node = new Node(points[median], dim);
    
    // Recursively build left and right subtrees
    node->left = buildTree(points, depth + 1, start, median - 1);
    node->right = buildTree(points, depth + 1, median + 1, end);
    
    return node;
}

template<typename T>
void KDTree3D<T>::nearestNeighborHelper(Node* node, const Point3D<T>& target, 
                                       Point3D<T>& best, T& bestDist) const {
    if (!node) return;
    
    // Calculate distance to current node
    T dist = distanceSquared(node->point, target);
    if (dist < bestDist) {
        bestDist = dist;
        best = node->point;
    }
    
    // Determine which side to search first
    bool goLeft = target[node->dimension] < node->point[node->dimension];
    Node* nearSide = goLeft ? node->left : node->right;
    Node* farSide = goLeft ? node->right : node->left;
    
    // Search near side first
    nearestNeighborHelper(nearSide, target, best, bestDist);
    
    // Check if we need to search far side
    T dimDist = target[node->dimension] - node->point[node->dimension];
    if (dimDist * dimDist < bestDist) {
        nearestNeighborHelper(farSide, target, best, bestDist);
    }
}

template<typename T>
void KDTree3D<T>::rangeSearchHelper(Node* node, const Point3D<T>& min, const Point3D<T>& max,
                                   std::vector<Point3D<T>>& result) const {
    if (!node) return;
    
    // Check if current point is in range
    if (node->point.x >= min.x && node->point.x <= max.x &&
        node->point.y >= min.y && node->point.y <= max.y &&
        node->point.z >= min.z && node->point.z <= max.z) {
        result.push_back(node->point);
    }
    
    // Determine which subtrees to search
    if (min[node->dimension] <= node->point[node->dimension]) {
        rangeSearchHelper(node->left, min, max, result);
    }
    if (max[node->dimension] >= node->point[node->dimension]) {
        rangeSearchHelper(node->right, min, max, result);
    }
}

template<typename T>
T KDTree3D<T>::distanceSquared(const Point3D<T>& a, const Point3D<T>& b) const {
    T dx = a.x - b.x;
    T dy = a.y - b.y;
    T dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

template<typename T>
int KDTree3D<T>::getDepth(Node* node) const {
    if (!node) return 0;
    return 1 + std::max(getDepth(node->left), getDepth(node->right));
}

template<typename T>
void KDTree3D<T>::build(std::vector<Point3D<T>> points) {
    delete root;
    if (points.empty()) {
        root = nullptr;
        return;
    }
    root = buildTree(points, 0, 0, points.size() - 1);
}

template<typename T>
void KDTree3D<T>::setBounds(const Point3D<T>& min, const Point3D<T>& max) {
    minBounds = min;
    maxBounds = max;
}

template<typename T>
Point3D<T> KDTree3D<T>::nearestNeighbor(const Point3D<T>& target) const {
    if (!root) return Point3D<T>();
    
    Point3D<T> best = root->point;
    T bestDist = distanceSquared(root->point, target);
    
    nearestNeighborHelper(root, target, best, bestDist);
    return best;
}

template<typename T>
std::vector<Point3D<T>> KDTree3D<T>::rangeSearch(const Point3D<T>& min, const Point3D<T>& max) const {
    std::vector<Point3D<T>> result;
    rangeSearchHelper(root, min, max, result);
    return result;
}

template<typename T>
std::vector<Point3D<T>> KDTree3D<T>::radiusSearch(const Point3D<T>& center, T radius) const {
    std::vector<Point3D<T>> result;
    T radiusSquared = radius * radius;
    
    // Use range search with bounding box first, then filter
    Point3D<T> min(center.x - radius, center.y - radius, center.z - radius);
    Point3D<T> max(center.x + radius, center.y + radius, center.z + radius);
    
    std::vector<Point3D<T>> candidates = rangeSearch(min, max);
    
    for (const auto& point : candidates) {
        if (distanceSquared(point, center) <= radiusSquared) {
            result.push_back(point);
        }
    }
    
    return result;
}

template<typename T>
bool KDTree3D<T>::empty() const {
    return root == nullptr;
}

template<typename T>
int KDTree3D<T>::depth() const {
    return getDepth(root);
}

// Explicit template instantiations for common types
template class Point3D<float>;
template class Point3D<double>;
template class Point3D<int>;

template class KDTree3D<float>;
template class KDTree3D<double>;
template class KDTree3D<int>;