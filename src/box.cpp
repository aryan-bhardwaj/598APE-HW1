#include "box.h"

Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty):Plane(c, t, ya, pi, ro, tx, ty){
   inBVH = false;
}
Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx):Plane(c, t, ya, pi, ro, tx,tx){
   inBVH = false;
}

double Box::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*time-center);
   return (std::abs(dist.x) > textureX / 2 || std::abs(dist.y) > textureY / 2)?inf:time;
}

bool Box::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;

   if(r<=0. || r>=1.) return false;
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*r-center);

   if (std::abs(dist.x) > textureX / 2 || std::abs(dist.y) > textureY / 2) return false;

   if(texture->opacity>1-1E-6) return true;

   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dist.x/textureX-.5), fix(dist.y/textureY-.5));
   if(op>1-1E-6) return true;

   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}

AABB Box::getBoundingBox() {
    // Compute the 4 corners of the box
    Vector corner1 = center + (right * (textureX / 2)) + (up * (textureY / 2));
    Vector corner2 = center - (right * (textureX / 2)) + (up * (textureY / 2));
    Vector corner3 = center + (right * (textureX / 2)) - (up * (textureY / 2));
    Vector corner4 = center - (right * (textureX / 2)) - (up * (textureY / 2));

    // Initialize min and max vectors
    Vector minCorner = corner1;
    Vector maxCorner = corner1;

    // Update min and max based on all corners
    for (Vector corner : {corner2, corner3, corner4}) {
        minCorner.x = std::min(minCorner.x, corner.x);
        minCorner.y = std::min(minCorner.y, corner.y);
        minCorner.z = std::min(minCorner.z, corner.z);

        maxCorner.x = std::max(maxCorner.x, corner.x);
        maxCorner.y = std::max(maxCorner.y, corner.y);
        maxCorner.z = std::max(maxCorner.z, corner.z);
    }

    // Return the AABB (assuming AABB is a struct with min/max points)
    return AABB(minCorner, maxCorner);
}
