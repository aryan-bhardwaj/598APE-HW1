
#include "light.h"
#include "shape.h"
#include "camera.h"
#include <algorithm>
#include "aabb.cpp"
      
Light::Light(const Vector & cente, unsigned char* colo) : center(cente){
   color = colo;
}

unsigned char* Light::getColor(unsigned char a, unsigned char b, unsigned char c){
   unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*3);
   r[0] = a;
   r[1] = b;
   r[2] = c;
   return r;
}

Autonoma::Autonoma(const Camera& c): camera(c){
   bvhRoot = NULL;
   depth = 10;
   skybox = BLACK;
}

Autonoma::Autonoma(const Camera& c, Texture* tex): camera(c){
   bvhRoot = NULL;
   depth = 10;
   skybox = tex;
}

void Autonoma::addShape(Shape* r){
   shapes.push_back(r);
}

void Autonoma::removeShape(Shape* s){
   shapes.erase(find(shapes.begin(), shapes.end(), s));
}

void Autonoma::addLight(Light* r){
   lights.push_back(r);
}

void Autonoma::removeLight(Light* s){
   lights.erase(find(lights.begin(), lights.end(), s));
}

BVHNode* Autonoma::buildBVH(std::vector<Shape*>& shapes, int start, int end) {
   BVHNode* node = new BVHNode();
   if (bvhRoot == NULL) {
      bvhRoot = node;
   }

   // Compute bounding box for all shapes in this range
   for (int i = start; i < end; i++) {
      if (shapes[i]->inBVH) {
         node->box.expand(shapes[i]->getBoundingBox());
      }
   }

   int numShapes = end - start;
   if (numShapes <= 2) { // Leaf node condition (adjust threshold as needed)
       for (int i = start; i < end; i++) {
         if (shapes[i]->inBVH) {
            node->shapes.push_back(shapes[i]);
         }
       }
       return node;
   }

   // Find the longest axis to split
   Vector size = node->box.max - node->box.min;
   int axis = (size.x > size.y && size.x > size.z) ? 0 : (size.y > size.z ? 1 : 2);

   // Sort shapes by centroid along the chosen axis
   std::sort(shapes.begin() + start, shapes.begin() + end, [&](Shape* a, Shape* b) {
       return a->getBoundingBox().min[axis] < b->getBoundingBox().min[axis];
   });

   // Split into two halves
   int mid = start + numShapes / 2;
   node->left = buildBVH(shapes, start, mid);
   node->right = buildBVH(shapes, mid, end);

   return node;
}


TimeAndShape Autonoma::intersectBVH(Ray ray) {
   return intersectBVHRecursive(ray, bvhRoot);
}

TimeAndShape Autonoma::intersectBVHRecursive(Ray ray, BVHNode* node) {
   // if (!node || !node->intersect(ray)) return inf;
   if (!node || !node->intersect(ray)) return (TimeAndShape){inf, NULL};

   // Check if leaf node has no shapes in it (possible with our BVH implementation)
   if (!node->left && !node->right && node->shapes.empty()) {
      return (TimeAndShape){inf, NULL};
   }

   // If this is a leaf node, check for intersection with shapes
   if (node->shapes.size() > 0) {
      double nearest_time = inf;
      Shape* nearest_shape = NULL;
      for (Shape* shape : node->shapes) {
            double time = shape->getIntersection(ray);
            if (time < nearest_time) {
               nearest_time = time;
               nearest_shape = shape;
            }
      }
      // return nearest;
      return (TimeAndShape){nearest_time, nearest_shape};
   }

   // Recursively check left and right children
   // double leftTime = intersectBVHRecursive(ray, node->left);
   // double rightTime = intersectBVHRecursive(ray, node->right);
   TimeAndShape leftTimeAndShape = intersectBVHRecursive(ray, node->left);
   TimeAndShape rightTimeAndShape = intersectBVHRecursive(ray, node->right);

   if (leftTimeAndShape.time < rightTimeAndShape.time) {
      return leftTimeAndShape;
   }
   return rightTimeAndShape;
}

void getLight(double* tColor, Autonoma* aut, Vector& point, Vector norm, unsigned char flip){
   tColor[0] = tColor[1] = tColor[2] = 0.;
   for (Light* light : aut->lights) {
      double lightColor[3];     
      lightColor[0] = light->color[0]/255.;
      lightColor[1] = light->color[1]/255.;
      lightColor[2] = light->color[2]/255.;

      Vector ra = light->center-point;
      bool hit = false;

      for (Shape* shape : aut->shapes) {
         if (hit) {
            break;
         } else {
            hit = shape->getLightIntersection(Ray(point+ra*.01, ra), lightColor);
         }
      }
      double perc = (norm.dot(ra)/(ra.mag()*norm.mag()));
      if(!hit){
         if(flip && perc<0) perc=-perc;
         if(perc>0){
            tColor[0]+= perc*(lightColor[0]);
            if(tColor[0]>1.) tColor[0] = 1.;
            tColor[1]+= perc*(lightColor[0]);
            if(tColor[1]>1.) tColor[1] = 1.;
            tColor[2]+= perc*(lightColor[0]);
            if(tColor[2]>1.) tColor[2] = 1.;
         }
      }
   }
}
