#include "aabb.cpp"
#include "shape.h"
#include <vector>

class BVHNode {
public:
    AABB box;             // Bounding box for the node
    BVHNode* left;        // Left child
    BVHNode* right;       // Right child
    std::vector<Shape*> shapes; // Shapes (only for leaf nodes)

    BVHNode() : left(nullptr), right(nullptr) {}

    // Check if this node intersects with the ray
    bool intersect(const Ray& ray) {
        return box.intersect(ray);
    }
};
