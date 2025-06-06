#include "./kd_tree_space_partition.h"

template<typename T>
KDTreeSimple<T>::KDTreeSimple(const Point3D<T>& minBounds, const Point3D<T>& maxBounds, int maxDepth_)
    : worldBounds(minBounds, maxBounds), maxDepth(maxDepth_), currentSize(0) {
    root = createNode(worldBounds, 0);
}

template<typename T>
KDTreeSimple<T>::~KDTreeSimple() {
    delete root;
}

template<typename T>
typename KDTreeSimple<T>::Node* KDTreeSimple<T>::createNode(const BoundingBox<T>& bounds, int depth) {
    if (depth >= maxDepth) {
        // At max depth, create a node that can hold a point but has no split
        return new Node(bounds, -1, 0);  // dimension -1 means no split
    }
    
    int dim = depth % 3;  // cycle through x, y, z
    T splitValue = (bounds.min[dim] + bounds.max[dim]) / 2;
    
    return new Node(bounds, dim, splitValue);
}

template<typename T>
void KDTreeSimple<T>::insertPoint(Node* node, const Point3D<T>& point, int depth) {
    // If this node doesn't have a point yet, store it here
    if (!node->hasPoint) {
        node->point = point;
        node->hasPoint = true;
        currentSize++;
        return;
    }
    
    // If we're at max depth, we can't split further - this is a collision
    if (depth >= maxDepth || node->dimension == -1) {
        // Could handle collisions differently, but for now just ignore
        return;
    }
    
    // Node already has a point, so we need to split
    // Create children if they don't exist
    if (!node->left) {
        BoundingBox<T> leftBounds = node->bounds;
        leftBounds.max[node->dimension] = node->splitValue;
        node->left = createNode(leftBounds, depth + 1);
        
        BoundingBox<T> rightBounds = node->bounds;
        rightBounds.min[node->dimension] = node->splitValue;
        node->right = createNode(rightBounds, depth + 1);
    }
    
    // Decide which child gets the new point
    if (point[node->dimension] <= node->splitValue) {
        insertPoint(node->left, point, depth + 1);
    } else {
        insertPoint(node->right, point, depth + 1);
    }
}

template<typename T>
void KDTreeSimple<T>::nearestNeighborHelper(Node* node, const Point3D<T>& target, 
                                           Point3D<T>& best, T& bestDist) const {
    if (!node) return;
    
    // Early termination if node is too far away
    T nodeDist = pointToBoxDistanceSquared(target, node->bounds);
    if (nodeDist >= bestDist) return;
    
    // Check if this node has a point and if it's better
    if (node->hasPoint) {
        T dist = distanceSquared(node->point, target);
        if (dist < bestDist) {
            bestDist = dist;
            best = node->point;
        }
    }
    
    // If this is a splitting node, search children
    if (node->dimension != -1 && node->left && node->right) {
        // Search closer child first
        bool goLeft = target[node->dimension] <= node->splitValue;
        Node* nearChild = goLeft ? node->left : node->right;
        Node* farChild = goLeft ? node->right : node->left;
        
        nearestNeighborHelper(nearChild, target, best, bestDist);
        nearestNeighborHelper(farChild, target, best, bestDist);
    }
}

template<typename T>
void KDTreeSimple<T>::rangeSearchHelper(Node* node, const BoundingBox<T>& range,
                                       std::vector<Point3D<T>>& result) const {
    if (!node || !node->bounds.intersects(range)) return;
    
    // Check if this node's point is in range
    if (node->hasPoint && range.contains(node->point)) {
        result.push_back(node->point);
    }
    
    // Search children
    if (node->left) rangeSearchHelper(node->left, range, result);
    if (node->right) rangeSearchHelper(node->right, range, result);
}

template<typename T>
void KDTreeSimple<T>::radiusSearchHelper(Node* node, const Point3D<T>& center, T radiusSquared,
                                        std::vector<Point3D<T>>& result) const {
    if (!node || !node->bounds.intersectsSphere(center, std::sqrt(radiusSquared))) return;
    
    // Check if this node's point is within radius
    if (node->hasPoint && distanceSquared(node->point, center) <= radiusSquared) {
        result.push_back(node->point);
    }
    
    // Search children
    if (node->left) radiusSearchHelper(node->left, center, radiusSquared, result);
    if (node->right) radiusSearchHelper(node->right, center, radiusSquared, result);
}

template<typename T>
T KDTreeSimple<T>::distanceSquared(const Point3D<T>& a, const Point3D<T>& b) const {
    T dx = a.x - b.x;
    T dy = a.y - b.y;
    T dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

template<typename T>
T KDTreeSimple<T>::pointToBoxDistanceSquared(const Point3D<T>& point, const BoundingBox<T>& box) const {
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
int KDTreeSimple<T>::getDepth(Node* node) const {
    if (!node) return 0;
    if (!node->left && !node->right) return 1;
    return 1 + std::max(getDepth(node->left), getDepth(node->right));
}

template<typename T>
int KDTreeSimple<T>::countPoints(Node* node) const {
    if (!node) return 0;
    int count = node->hasPoint ? 1 : 0;
    return count + countPoints(node->left) + countPoints(node->right);
}

// Public interface
template<typename T>
void KDTreeSimple<T>::insert(const Point3D<T>& point) {
    if (worldBounds.contains(point)) {
        insertPoint(root, point, 0);
    }
}

template<typename T>
void KDTreeSimple<T>::insert(const std::vector<Point3D<T>>& points) {
    for (const auto& point : points) {
        insert(point);
    }
}

template<typename T>
Point3D<T> KDTreeSimple<T>::nearestNeighbor(const Point3D<T>& target) const {
    if (empty()) return Point3D<T>();
    
    Point3D<T> best;
    T bestDist = std::numeric_limits<T>::max();
    
    nearestNeighborHelper(root, target, best, bestDist);
    return best;
}

template<typename T>
std::vector<Point3D<T>> KDTreeSimple<T>::rangeSearch(const Point3D<T>& min, const Point3D<T>& max) const {
    std::vector<Point3D<T>> result;
    BoundingBox<T> range(min, max);
    rangeSearchHelper(root, range, result);
    return result;
}

template<typename T>
std::vector<Point3D<T>> KDTreeSimple<T>::radiusSearch(const Point3D<T>& center, T radius) const {
    std::vector<Point3D<T>> result;
    radiusSearchHelper(root, center, radius * radius, result);
    return result;
}

template<typename T>
bool KDTreeSimple<T>::empty() const {
    return currentSize == 0;
}

template<typename T>
int KDTreeSimple<T>::size() const {
    return currentSize;
}

template<typename T>
int KDTreeSimple<T>::depth() const {
    return getDepth(root);
}

template<typename T>
void KDTreeSimple<T>::clear() {
    delete root;
    root = createNode(worldBounds, 0);
    currentSize = 0;
}

template<typename T>
BoundingBox<T> KDTreeSimple<T>::getBounds() const {
    return worldBounds;
}

// Explicit template instantiations
template class KDTreeSimple<float>;
template class KDTreeSimple<double>;
template class KDTreeSimple<int>;