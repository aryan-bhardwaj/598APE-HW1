#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vector.h"
#include "camera.h"
#include "Textures/texture.h"
#include "Textures/colortexture.h"
#include <vector>

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

   Autonoma(const Camera &c);
   Autonoma(const Camera &c, Texture* tex);
   void addShape(Shape* s);
   void removeShape(Shape* s);
   void addLight(Light* s);
   void removeLight(Light* s);
};

void getLight(double* toFill, Autonoma* aut, Vector& point, Vector norm, unsigned char r);

#endif
