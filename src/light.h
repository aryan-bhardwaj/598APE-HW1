#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vector.h"
#include "camera.h"
#include "Textures/texture.h"
#include "Textures/colortexture.h"
#include <vector>
#include "BVHNode.cpp"

class Light{
  public:
   unsigned char* color;
   unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c);
   Vector center;
   Light(const Vector & cente, unsigned char* colo);
};

class Shape;

class Autonoma{
public:
   Camera camera;
   Texture* skybox;
   unsigned int depth;

   std::vector<Shape*> shapes;
   std::vector<Light*> lights;

   BVHNode* bvhRoot;           // Root of the BVH

   Autonoma(const Camera &c);
   Autonoma(const Camera &c, Texture* tex);
   void addShape(Shape* s);
   void removeShape(Shape* s);
   void addLight(Light* s);
   void removeLight(Light* s);

   // BVH functions
   BVHNode* buildBVH(std::vector<Shape*>& shapes, int start, int end);     // function to build the BVH tree
   TimeAndShape Autonoma::intersectBVH(Ray ray);
   TimeAndShape Autonoma::intersectBVHRecursive(Ray ray, BVHNode* node);
};

struct TimeAndShape {
   double time;
   Shape* shape;
};

void getLight(double* toFill, Autonoma* aut, Vector& point, Vector norm, unsigned char r);

#endif
