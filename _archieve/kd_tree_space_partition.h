#ifndef KDTREESPACE_H
#define KDTREESPACE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

template<typename T>
struct Point3DSpace {
    T x, y, z;
    
    Point3DSpace();
    Point3DSpace(T x_, T y_, T z_);
    
    T& operator[](int dim);
    const T& operator[](int dim) const;
    
    bool operator==(const Point3DSpace<T>& other) const;
};

template<typename T>
struct BoundingBox {
    Point3DSpace<T> min, max;
    
    BoundingBox();
    BoundingBox(const Point3DSpace<T>& min_, const Point3DSpace<T>& max_);
    
    Point3DSpace<T> center() const;
    T size(int dim) const;
    bool contains(const Point3DSpace<T>& point) const;
    bool intersects(const BoundingBox<T>& other) const;
    bool intersectsSphere(const Point3DSpace<T>& center, T radius) const;
};

template<typename T>
class KDTree3DSpace {
private:
    struct Node {
        BoundingBox<T> bounds;
        int dimension;  // splitting dimension (0=x, 1=y, 2=z)
        T splitValue;   // predetermined split position
        Node* left;
        Node* right;
        std::vector<Point3DSpace<T>> points;  // only used in leaf nodes
        bool isLeaf;
        
        Node(const BoundingBox<T>& bounds_, int dim, T split);
        Node(const BoundingBox<T>& bounds_);  // leaf constructor
        ~Node();
    };
    
    Node* root;
    BoundingBox<T> worldBounds;
    int maxDepth;
    int maxPointsPerLeaf;
    
    // Private helper methods
    Node* createNode(const BoundingBox<T>& bounds, int depth);
    void insertPoint(Node* node, const Point3DSpace<T>& point, int depth);
    void nearestNeighborHelper(Node* node, const Point3DSpace<T>& target, 
                              Point3DSpace<T>& best, T& bestDist) const;
    void rangeSearchHelper(Node* node, const BoundingBox<T>& range,
                          std::vector<Point3DSpace<T>>& result) const;
    void radiusSearchHelper(Node* node, const Point3DSpace<T>& center, T radiusSquared,
                           std::vector<Point3DSpace<T>>& result) const;
    T distanceSquared(const Point3DSpace<T>& a, const Point3DSpace<T>& b) const;
    T pointToBoxDistanceSquared(const Point3DSpace<T>& point, const BoundingBox<T>& box) const;
    int getDepth(Node* node) const;
    int countNodes(Node* node) const;
    
public:
    KDTree3DSpace(const Point3DSpace<T>& minBounds, const Point3DSpace<T>& maxBounds, 
             int maxDepth_ = 10, int maxPointsPerLeaf_ = 8);
    ~KDTree3DSpace();
    
    // Copy constructor and assignment operator (deleted for simplicity)
    KDTree3DSpace(const KDTree3DSpace&) = delete;
    KDTree3DSpace& operator=(const KDTree3DSpace&) = delete;
    
    // Public interface
    void insert(const Point3DSpace<T>& point);
    void insert(const std::vector<Point3DSpace<T>>& points);
    Point3DSpace<T> nearestNeighbor(const Point3DSpace<T>& target) const;
    std::vector<Point3DSpace<T>> rangeSearch(const Point3DSpace<T>& min, const Point3DSpace<T>& max) const;
    std::vector<Point3DSpace<T>> radiusSearch(const Point3DSpace<T>& center, T radius) const;
    bool empty() const;
    int depth() const;
    int size() const;
    void clear();
    
    // Debug information
    BoundingBox<T> getBounds() const;
    int getMaxDepth() const;
    int getMaxPointsPerLeaf() const;
};

#endif