#ifndef AABB_H
#define AABB_H

#include "vector.h"
#include <algorithm>

class AABB {
public:
    Vector min, max;

    AABB() : min(Vector(inf, inf, inf)), max(Vector(-inf, -inf, -inf)) {} // Initialize as an invalid box
    AABB(const Vector& min, const Vector& max) : min(min), max(max) {}

    // Expand this AABB to include another AABB
    void expand(const AABB& other) {
        min.x = std::min(min.x, other.min.x);
        min.y = std::min(min.y, other.min.y);
        min.z = std::min(min.z, other.min.z);
        
        max.x = std::max(max.x, other.max.x);
        max.y = std::max(max.y, other.max.y);
        max.z = std::max(max.z, other.max.z);
    }

    // Expand to include a point
    void expand(const Vector& point) {
        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);

        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }

    // Checks if a ray intersects this AABB (using slab method)
    bool intersect(const Ray& ray) const {
        double tmin = (min.x - ray.point.x) / ray.vector.x;
        double tmax = (max.x - ray.point.x) / ray.vector.x;
        if (tmin > tmax) std::swap(tmin, tmax);

        double tymin = (min.y - ray.point.y) / ray.vector.y;
        double tymax = (max.y - ray.point.y) / ray.vector.y;
        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return false;
        
        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        double tzmin = (min.z - ray.point.z) / ray.vector.z;
        double tzmax = (max.z - ray.point.z) / ray.vector.z;
        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        
        return true;
    }
};

#endif