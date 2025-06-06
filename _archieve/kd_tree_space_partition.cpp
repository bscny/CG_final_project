#include "./kd_tree_space_partition.h"

// Point3DSpace implementation
template<typename T>
Point3DSpace<T>::Point3DSpace() : x(0), y(0), z(0) {}

template<typename T>
Point3DSpace<T>::Point3DSpace(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

template<typename T>
T& Point3DSpace<T>::operator[](int dim) {
    return (dim == 0) ? x : (dim == 1) ? y : z;
}

template<typename T>
const T& Point3DSpace<T>::operator[](int dim) const {
    return (dim == 0) ? x : (dim == 1) ? y : z;
}

template<typename T>
bool Point3DSpace<T>::operator==(const Point3DSpace<T>& other) const {
    return x == other.x && y == other.y && z == other.z;
}

// BoundingBox implementation
template<typename T>
BoundingBox<T>::BoundingBox() {}

template<typename T>
BoundingBox<T>::BoundingBox(const Point3DSpace<T>& min_, const Point3DSpace<T>& max_) 
    : min(min_), max(max_) {}

template<typename T>
Point3DSpace<T> BoundingBox<T>::center() const {
    return Point3DSpace<T>((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
}

template<typename T>
T BoundingBox<T>::size(int dim) const {
    return max[dim] - min[dim];
}

template<typename T>
bool BoundingBox<T>::contains(const Point3DSpace<T>& point) const {
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

template<typename T>
bool BoundingBox<T>::intersects(const BoundingBox<T>& other) const {
    return !(other.min.x > max.x || other.max.x < min.x ||
             other.min.y > max.y || other.max.y < min.y ||
             other.min.z > max.z || other.max.z < min.z);
}

template<typename T>
bool BoundingBox<T>::intersectsSphere(const Point3DSpace<T>& center, T radius) const {
    T distSq = 0;
    
    // Calculate squared distance from sphere center to box
    for (int i = 0; i < 3; ++i) {
        if (center[i] < min[i]) {
            T diff = min[i] - center[i];
            distSq += diff * diff;
        } else if (center[i] > max[i]) {
            T diff = center[i] - max[i];
            distSq += diff * diff;
        }
    }
    
    return distSq <= radius * radius;
}

// Node implementation
template<typename T>
KDTree3DSpace<T>::Node::Node(const BoundingBox<T>& bounds_, int dim, T split)
    : bounds(bounds_), dimension(dim), splitValue(split), left(nullptr), right(nullptr), isLeaf(false) {}

template<typename T>
KDTree3DSpace<T>::Node::Node(const BoundingBox<T>& bounds_)
    : bounds(bounds_), dimension(-1), splitValue(0), left(nullptr), right(nullptr), isLeaf(true) {}

template<typename T>
KDTree3DSpace<T>::Node::~Node() {
    delete left;
    delete right;
}

// KDTree3DSpace implementation
template<typename T>
KDTree3DSpace<T>::KDTree3DSpace(const Point3DSpace<T>& minBounds, const Point3DSpace<T>& maxBounds, 
                      int maxDepth_, int maxPointsPerLeaf_)
    : worldBounds(minBounds, maxBounds), maxDepth(maxDepth_), maxPointsPerLeaf(maxPointsPerLeaf_) {
    root = createNode(worldBounds, 0);
}

template<typename T>
KDTree3DSpace<T>::~KDTree3DSpace() {
    delete root;
}

template<typename T>
typename KDTree3DSpace<T>::Node* KDTree3DSpace<T>::createNode(const BoundingBox<T>& bounds, int depth) {
    // Create leaf node if max depth reached
    if (depth >= maxDepth) {
        return new Node(bounds);  // leaf constructor
    }
    
    int dim = depth % 3;  // cycle through x, y, z
    T splitValue = (bounds.min[dim] + bounds.max[dim]) / 2;
    
    return new Node(bounds, dim, splitValue);
}

template<typename T>
void KDTree3DSpace<T>::insertPoint(Node* node, const Point3DSpace<T>& point, int depth) {
    if (node->isLeaf) {
        node->points.push_back(point);
        
        // Split leaf if it has too many points and we haven't reached max depth
        if (node->points.size() > maxPointsPerLeaf && depth < maxDepth) {
            int dim = depth % 3;
            T splitValue = (node->bounds.min[dim] + node->bounds.max[dim]) / 2;
            
            // Create left and right child bounds
            BoundingBox<T> leftBounds = node->bounds;
            BoundingBox<T> rightBounds = node->bounds;
            leftBounds.max[dim] = splitValue;
            rightBounds.min[dim] = splitValue;
            
            // Create children
            node->left = createNode(leftBounds, depth + 1);
            node->right = createNode(rightBounds, depth + 1);
            node->dimension = dim;
            node->splitValue = splitValue;
            node->isLeaf = false;
            
            // Redistribute points to children
            for (const auto& p : node->points) {
                if (p[dim] <= splitValue) {
                    insertPoint(node->left, p, depth + 1);
                } else {
                    insertPoint(node->right, p, depth + 1);
                }
            }
            
            node->points.clear();  // Clear points from internal node
        }
    } else {
        // Internal node - route to appropriate child
        if (point[node->dimension] <= node->splitValue) {
            insertPoint(node->left, point, depth + 1);
        } else {
            insertPoint(node->right, point, depth + 1);
        }
    }
}

template<typename T>
void KDTree3DSpace<T>::nearestNeighborHelper(Node* node, const Point3DSpace<T>& target, 
                                       Point3DSpace<T>& best, T& bestDist) const {
    if (!node) return;
    
    // Early termination if node is too far away
    T nodeDist = pointToBoxDistanceSquared(target, node->bounds);
    if (nodeDist >= bestDist) return;
    
    if (node->isLeaf) {
        // Check all points in leaf
        for (const auto& point : node->points) {
            T dist = distanceSquared(point, target);
            if (dist < bestDist) {
                bestDist = dist;
                best = point;
            }
        }
    } else {
        // Internal node - search both children, but prioritize closer one
        bool goLeft = target[node->dimension] <= node->splitValue;
        Node* nearChild = goLeft ? node->left : node->right;
        Node* farChild = goLeft ? node->right : node->left;
        
        // Search near child first
        nearestNeighborHelper(nearChild, target, best, bestDist);
        
        // Search far child only if it might contain a better point
        nearestNeighborHelper(farChild, target, best, bestDist);
    }
}

template<typename T>
void KDTree3DSpace<T>::rangeSearchHelper(Node* node, const BoundingBox<T>& range,
                                   std::vector<Point3DSpace<T>>& result) const {
    if (!node || !node->bounds.intersects(range)) return;
    
    if (node->isLeaf) {
        // Check all points in leaf
        for (const auto& point : node->points) {
            if (range.contains(point)) {
                result.push_back(point);
            }
        }
    } else {
        // Search both children
        rangeSearchHelper(node->left, range, result);
        rangeSearchHelper(node->right, range, result);
    }
}

template<typename T>
void KDTree3DSpace<T>::radiusSearchHelper(Node* node, const Point3DSpace<T>& center, T radiusSquared,
                                     std::vector<Point3DSpace<T>>& result) const {
    if (!node || !node->bounds.intersectsSphere(center, std::sqrt(radiusSquared))) return;
    
    if (node->isLeaf) {
        // Check all points in leaf
        for (const auto& point : node->points) {
            if (distanceSquared(point, center) <= radiusSquared) {
                result.push_back(point);
            }
        }
    } else {
        // Search both children
        radiusSearchHelper(node->left, center, radiusSquared, result);
        radiusSearchHelper(node->right, center, radiusSquared, result);
    }
}

template<typename T>
T KDTree3DSpace<T>::distanceSquared(const Point3DSpace<T>& a, const Point3DSpace<T>& b) const {
    T dx = a.x - b.x;
    T dy = a.y - b.y;
    T dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

template<typename T>
T KDTree3DSpace<T>::pointToBoxDistanceSquared(const Point3DSpace<T>& point, const BoundingBox<T>& box) const {
    T distSq = 0;
    
    for (int i = 0; i < 3; ++i) {
        if (point[i] < box.min[i]) {
            T diff = box.min[i] - point[i];
            distSq += diff * diff;
        } else if (point[i] > box.max[i]) {
            T diff = point[i] - box.max[i];
            distSq += diff * diff;
        }
    }
    
    return distSq;
}

template<typename T>
int KDTree3DSpace<T>::getDepth(Node* node) const {
    if (!node) return 0;
    if (node->isLeaf) return 1;
    return 1 + std::max(getDepth(node->left), getDepth(node->right));
}

template<typename T>
int KDTree3DSpace<T>::countNodes(Node* node) const {
    if (!node) return 0;
    if (node->isLeaf) return node->points.size();
    return countNodes(node->left) + countNodes(node->right);
}

// Public interface
template<typename T>
void KDTree3DSpace<T>::insert(const Point3DSpace<T>& point) {
    if (worldBounds.contains(point)) {
        insertPoint(root, point, 0);
    }
}

template<typename T>
void KDTree3DSpace<T>::insert(const std::vector<Point3DSpace<T>>& points) {
    for (const auto& point : points) {
        insert(point);
    }
}

template<typename T>
Point3DSpace<T> KDTree3DSpace<T>::nearestNeighbor(const Point3DSpace<T>& target) const {
    if (!root || empty()) return Point3DSpace<T>();
    
    Point3DSpace<T> best;
    T bestDist = std::numeric_limits<T>::max();
    
    nearestNeighborHelper(root, target, best, bestDist);
    return best;
}

template<typename T>
std::vector<Point3DSpace<T>> KDTree3DSpace<T>::rangeSearch(const Point3DSpace<T>& min, const Point3DSpace<T>& max) const {
    std::vector<Point3DSpace<T>> result;
    BoundingBox<T> range(min, max);
    rangeSearchHelper(root, range, result);
    return result;
}

template<typename T>
std::vector<Point3DSpace<T>> KDTree3DSpace<T>::radiusSearch(const Point3DSpace<T>& center, T radius) const {
    std::vector<Point3DSpace<T>> result;
    radiusSearchHelper(root, center, radius * radius, result);
    return result;
}

template<typename T>
bool KDTree3DSpace<T>::empty() const {
    return size() == 0;
}

template<typename T>
int KDTree3DSpace<T>::depth() const {
    return getDepth(root);
}

template<typename T>
int KDTree3DSpace<T>::size() const {
    return countNodes(root);
}

template<typename T>
void KDTree3DSpace<T>::clear() {
    delete root;
    root = createNode(worldBounds, 0);
}

template<typename T>
BoundingBox<T> KDTree3DSpace<T>::getBounds() const {
    return worldBounds;
}

template<typename T>
int KDTree3DSpace<T>::getMaxDepth() const {
    return maxDepth;
}

template<typename T>
int KDTree3DSpace<T>::getMaxPointsPerLeaf() const {
    return maxPointsPerLeaf;
}

// Explicit template instantiations
template class Point3DSpace<float>;
template class Point3DSpace<double>;
template class Point3DSpace<int>;

template class BoundingBox<float>;
template class BoundingBox<double>;
template class BoundingBox<int>;

template class KDTree3DSpace<float>;
template class KDTree3DSpace<double>;
template class KDTree3DSpace<int>;