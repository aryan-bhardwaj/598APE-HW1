#include "box.h"

Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty):Plane(c, t, ya, pi, ro, tx, ty){}
Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx):Plane(c, t, ya, pi, ro, tx,tx){}

double Box::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*time-center);
   if(time==inf) 
      return time;
   // return ( ((dist.x>=0)?dist.x:-dist.x)>textureX/2 || ((dist.y>=0)?dist.y:-dist.y)>textureY/2 )?inf:time;
   return (std::abs(dist.x) > textureX / 2 || std::abs(dist.y) > textureY / 2)?inf:time;
}

// TODO: optimize this lil guy, it's taking too long
bool Box::getLightIntersection(Ray ray, double* fill){
   // const double t = ray.vector.dot(vect);
   // const double norm = vect.dot(ray.point)+d;
   // const double r = -norm/t;
   const double norm = vect.dot(ray.point) + d;
   const double r = -norm / ray.vector.dot(vect);

   if(r<=0. || r>=1.) return false;
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*r-center);

   // if( ((dist.x>=0)?dist.x:-dist.x)>textureX/2 || ((dist.y>=0)?dist.y:-dist.y)>textureY/2 ) return false;
   // return !(std::abs(dist.x) <= textureX / 2 && std::abs(dist.y) <= textureY / 2);
   if (std::abs(dist.x) > textureX / 2 || std::abs(dist.y) > textureY / 2) return false;

   if(texture->opacity>1-1E-6) return true;
   // return (texture->opacity>1-1E-6);

   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dist.x/textureX-.5), fix(dist.y/textureY-.5));
   if(op>1-1E-6) return true;
   // return (op>1-1E-6);
   
   // trying to combine array accesses, or make this into a for loop and parallelize with cilk_for
   // unsigned char a = temp[0];
   // unsigned char b = temp[1];
   // unsigned char c = temp[2];
   // fill[0]*=a/255.;
   // fill[1]*=b/255.;
   // fill[2]*=c/255.;
   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}