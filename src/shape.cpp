#include "shape.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <chrono>

using namespace std::chrono;


Shape::Shape(const Vector &c, Texture* t, double ya, double pi, double ro): center(c), texture(t), yaw(ya), pitch(pi), roll(ro){
};

void Shape::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
}

void Shape::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
}

void Shape::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
}

void Shape::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
}

void calcColor(unsigned char* toFill, Autonoma* c, Ray ray, unsigned int depth){
   double minTime = inf;
   Shape* minShape = NULL;

   // auto startBVH = high_resolution_clock::now();

   // TODO: replace this loop with the BVH lookup
   TimeAndShape bvhResults = c->intersectBVH(ray);

   // auto endBVH = high_resolution_clock::now();
   // std::cout << "BVH intersection time: " 
   //           << duration_cast<microseconds>(endBVH - startBVH).count() << " µs" << std::endl;

   // auto startLoop = high_resolution_clock::now();


   if (bvhResults.time != inf) {
      // std::cout << "found with bvh" << std::endl;
      minTime = bvhResults.time;
      minShape = bvhResults.shape;
   } else {    // we weren't able to find the shape in the bounding box
      // std::cout << "did not find with bvh" << std::endl;
      for (Shape* shape : c->shapes) {
         // std::cout << ray.point.x << std::endl;
         double time = shape->getIntersection(ray);
         if (time < minTime) {
            minTime = time;
            minShape = shape;
         }
      }
   }
   
   // auto endLoop = high_resolution_clock::now();
   // std::cout << "Shape intersection loop time: " 
   //           << duration_cast<microseconds>(endLoop - startLoop).count() << " µs" << std::endl;
   // std::cout << "min time found: " << minTime << std::endl;
   // std::cout << "min shape found: " << minShape << std::endl;
   
   if (minTime == inf) {
      double opacity, reflection, ambient;
      Vector temp = ray.vector.normalize();
      const double x = temp.x;
      const double z = temp.z;
      const double me = std::abs(temp.y);
      const double angle = atan2(z, x);
      c->skybox->getColor(toFill, &ambient, &opacity, &reflection, fix(angle/M_TWO_PI),fix(me));
      return;
   }

   double curTime = minTime;
   Shape* curShape = minShape;

   Vector intersect = curTime*ray.vector+ray.point;
   double opacity, reflection, ambient;
   curShape->getColor(toFill, &ambient, &opacity, &reflection, c, Ray(intersect, ray.vector), depth);
   
   double lightData[3];
   getLight(lightData, c, intersect, curShape->getNormal(intersect), curShape->reversible());
   toFill[0] = (unsigned char)(toFill[0]*(ambient+lightData[0]*(1-ambient)));
   toFill[1] = (unsigned char)(toFill[1]*(ambient+lightData[1]*(1-ambient)));
   toFill[2] = (unsigned char)(toFill[2]*(ambient+lightData[2]*(1-ambient)));
   if(depth<c->depth && (opacity<1-1e-6 || reflection>1e-6)){
      unsigned char col[4];
      if(opacity<1-1e-6){
         Ray nextRay = Ray(intersect+ray.vector*1E-4, ray.vector);
         calcColor(col, c, nextRay, depth+1);
         toFill[0]= (unsigned char)(toFill[0]*opacity+col[0]*(1-opacity));
         toFill[1]= (unsigned char)(toFill[1]*opacity+col[1]*(1-opacity));
         toFill[2]= (unsigned char)(toFill[2]*opacity+col[2]*(1-opacity));        
      }
      if(reflection>1e-6){
         Vector norm = curShape->getNormal(intersect).normalize();
         Vector vec = ray.vector-2*norm*(norm.dot(ray.vector));
         Ray nextRay = Ray(intersect+vec*1E-4, vec);
         calcColor(col, c, nextRay, depth+1);
      
         toFill[0]= (unsigned char)(toFill[0]*(1-reflection)+col[0]*(reflection));
         toFill[1]= (unsigned char)(toFill[1]*(1-reflection)+col[1]*(reflection));
         toFill[2]= (unsigned char)(toFill[2]*(1-reflection)+col[2]*(reflection));
      }
   }
}