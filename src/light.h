#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vector.h"
#include "camera.h"
#include "Textures/texture.h"
#include "Textures/colortexture.h"
#include <vector>
#include "aabb.cpp"
// #include "BVHNode.cpp"

class Light{
  public:
   unsigned char* color;
   unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c);
   Vector center;
   Light(const Vector & cente, unsigned char* colo);
};

class Shape;

struct TimeAndShape {
   double time;
   Shape* shape;
};

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

class Autonoma{
public:
   Camera camera;
   Texture* skybox;
   unsigned int depth;

   std::vector<Shape*> shapes;
   std::vector<Light*> lights;

   BVHNode* bvhRoot;           // Root of the BVH
   int numBVHshapes;
   std::vector<Shape*> nonBVHshapes;

   Autonoma(const Camera &c);
   Autonoma(const Camera &c, Texture* tex);
   void addShape(Shape* s);
   void removeShape(Shape* s);
   void addLight(Light* s);
   void removeLight(Light* s);

   // BVH functions
   BVHNode* buildBVH(std::vector<Shape*>& shapes, int start, int end);
   TimeAndShape intersectBVH(Ray ray);
   TimeAndShape intersectBVHRecursive(Ray ray, BVHNode* node);
};

void getLight(double* toFill, Autonoma* aut, Vector& point, Vector norm, unsigned char r);

#endif
