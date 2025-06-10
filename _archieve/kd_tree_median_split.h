#ifndef KDTREE3D_H
#define KDTREE3D_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

template<typename T>
struct Point3D {
    T x, y, z;
    
    Point3D();
    Point3D(T x_, T y_, T z_);
    
    T& operator[](int dim);
    const T& operator[](int dim) const;
};

template<typename T>
class KDTree3D {
private:
    struct Node {
        Point3D<T> point;
        int dimension;  // splitting dimension (0=x, 1=y, 2=z)
        Node* left;
        Node* right;
        
        Node(const Point3D<T>& p, int dim);
        ~Node();
    };
    
    Node* root;
    Point3D<T> minBounds, maxBounds;
    
    // Comparator for sorting points by dimension
    struct DimensionComparator {
        int dim;
        DimensionComparator(int d);
        bool operator()(const Point3D<T>& a, const Point3D<T>& b);
    };
    
    // Private helper methods
    Node* buildTree(std::vector<Point3D<T>>& points, int depth, int start, int end);
    void nearestNeighborHelper(Node* node, const Point3D<T>& target, 
                              Point3D<T>& best, T& bestDist) const;
    void rangeSearchHelper(Node* node, const Point3D<T>& min, const Point3D<T>& max,
                          std::vector<Point3D<T>>& result) const;
    T distanceSquared(const Point3D<T>& a, const Point3D<T>& b) const;
    int getDepth(Node* node) const;
    
public:
    KDTree3D();
    KDTree3D(const Point3D<T>& minBounds_, const Point3D<T>& maxBounds_);
    ~KDTree3D();
    
    // Copy constructor and assignment operator (deleted for simplicity)
    KDTree3D(const KDTree3D&) = delete;
    KDTree3D& operator=(const KDTree3D&) = delete;
    
    // Public interface
    void build(std::vector<Point3D<T>> points);
    void setBounds(const Point3D<T>& min, const Point3D<T>& max);
    Point3D<T> nearestNeighbor(const Point3D<T>& target) const;
    std::vector<Point3D<T>> rangeSearch(const Point3D<T>& min, const Point3D<T>& max) const;
    std::vector<Point3D<T>> radiusSearch(const Point3D<T>& center, T radius) const;
    bool empty() const;
    int depth() const;
};

#endif // KDTREE3D_H