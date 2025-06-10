#ifndef KD_TREE_SIMPLE_H
#define KD_TREE_SIMPLE_H

#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

template<typename T>
struct Point3D {
    T x, y, z;
    
    Point3D() : x(0), y(0), z(0) {}
    Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
    
    T& operator[](int dim) {
        return (dim == 0) ? x : (dim == 1) ? y : z;
    }
    
    const T& operator[](int dim) const {
        return (dim == 0) ? x : (dim == 1) ? y : z;
    }
    
    bool operator==(const Point3D<T>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

template<typename T>
struct BoundingBox {
    Point3D<T> min, max;
    
    BoundingBox() {}
    BoundingBox(const Point3D<T>& min_, const Point3D<T>& max_) : min(min_), max(max_) {}
    
    Point3D<T> center() const {
        return Point3D<T>((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
    }
    
    T size(int dim) const {
        return max[dim] - min[dim];
    }
    
    bool contains(const Point3D<T>& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }
    
    bool intersects(const BoundingBox<T>& other) const {
        return !(other.min.x > max.x || other.max.x < min.x ||
                 other.min.y > max.y || other.max.y < min.y ||
                 other.min.z > max.z || other.max.z < min.z);
    }
    
    bool intersectsSphere(const Point3D<T>& center, T radius) const {
        T distSq = 0;
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
};

template<typename T>
class KDTreeSimple {
private:
    struct Node {
        BoundingBox<T> bounds;
        Point3D<T> point;      // The actual point stored in this node
        bool hasPoint;         // Whether this node contains a point
        int dimension;         // Split dimension (0=x, 1=y, 2=z)
        T splitValue;          // Split value for this dimension
        Node* left;
        Node* right;
        
        // Constructor for empty spatial node
        Node(const BoundingBox<T>& bounds_, int dim, T split)
            : bounds(bounds_), hasPoint(false), dimension(dim), splitValue(split), 
              left(nullptr), right(nullptr) {}
        
        // Constructor for point-containing node
        Node(const BoundingBox<T>& bounds_, const Point3D<T>& p, int dim, T split)
            : bounds(bounds_), point(p), hasPoint(true), dimension(dim), splitValue(split),
              left(nullptr), right(nullptr) {}
        
        ~Node() {
            delete left;
            delete right;
        }
    };
    
    Node* root;
    BoundingBox<T> worldBounds;
    int maxDepth;
    int currentSize;
    
    Node* createNode(const BoundingBox<T>& bounds, int depth);
    void insertPoint(Node* node, const Point3D<T>& point, int depth);
    void nearestNeighborHelper(Node* node, const Point3D<T>& target, 
                             Point3D<T>& best, T& bestDist) const;
    void rangeSearchHelper(Node* node, const BoundingBox<T>& range,
                         std::vector<Point3D<T>>& result) const;
    void radiusSearchHelper(Node* node, const Point3D<T>& center, T radiusSquared,
                          std::vector<Point3D<T>>& result) const;
    
    T distanceSquared(const Point3D<T>& a, const Point3D<T>& b) const;
    T pointToBoxDistanceSquared(const Point3D<T>& point, const BoundingBox<T>& box) const;
    int getDepth(Node* node) const;
    int countPoints(Node* node) const;

public:
    KDTreeSimple(const Point3D<T>& minBounds, const Point3D<T>& maxBounds, int maxDepth_ = 20);
    ~KDTreeSimple();
    
    void insert(const Point3D<T>& point);
    void insert(const std::vector<Point3D<T>>& points);
    
    Point3D<T> nearestNeighbor(const Point3D<T>& target) const;
    std::vector<Point3D<T>> rangeSearch(const Point3D<T>& min, const Point3D<T>& max) const;
    std::vector<Point3D<T>> radiusSearch(const Point3D<T>& center, T radius) const;
    
    bool empty() const;
    int size() const;
    int depth() const;
    void clear();
    BoundingBox<T> getBounds() const;
};

#endif // KD_TREE_SIMPLE_H