#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vector.h"
#include "camera.h"
#include <vector>
#include "Textures/texture.h"
#include "Textures/colortexture.h"

class Light{
  public:
   unsigned char* color;
   unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c);
   Vector center;
   Light(const Vector & cente, unsigned char* colo);
};

// struct LightNode{
//    Light* data;
//    LightNode* prev, *next;
// };

class Shape;
// struct ShapeNode{
//    Shape* data;
//    ShapeNode* prev, *next;
// };

class Autonoma{
public:
   Camera camera;
   Texture* skybox;
   unsigned int depth;
   // ShapeNode *listStart, *listEnd;
   // LightNode *lightStart, *lightEnd;
   std::vector<Shape*> shapes;
   std::vector<Light*> lights;

   Autonoma(const Camera &c);
   Autonoma(const Camera &c, Texture* tex);
   void addShape(Shape* s);
   void removeShape(Shape* s);
   void addLight(Light* s);
   void removeLight(Light* s);
};

void getLight(double* toFill, Autonoma* aut, Vector point, Vector norm, unsigned char r);

#endif
