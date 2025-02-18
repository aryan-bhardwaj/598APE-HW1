#include<string.h>
#include<stdio.h>
#include<limits>
#include<math.h>
#include<stdlib.h>
//#include <printf.h>
#include <stddef.h>
#include "vector.h"
#include <bits/stdc++.h>
#include <iostream>

Vector::Vector(double a, double b, double c) : x(a), y(b), z(c) {
}
void Vector::operator -= (const Vector& rhs) {
   x-=rhs.x; y-=rhs.y; z-=rhs.z;
}
void Vector::operator += (const Vector& rhs) {
   x+=rhs.x; y+=rhs.y; z+=rhs.z;
}
void Vector::operator *= (const double rhs) {
   x*=rhs; y*=rhs; z*=rhs;
}
void Vector::operator *= (const float rhs) {
   x*=rhs; y*=rhs; z*=rhs;
}
void Vector::operator *= (const int rhs) {
   x*=rhs; y*=rhs; z*=rhs;
}
void Vector::operator /= (const double rhs) {
   x/=rhs; y/=rhs; z/=rhs;
}
void Vector::operator /= (const float rhs) {
   x/=rhs; y/=rhs; z/=rhs;
}
void Vector::operator /= (const int rhs) {
   x/=rhs; y/=rhs; z/=rhs;
}


Vector Vector::operator - (const Vector& rhs) {
   return Vector(x-rhs.x, y-rhs.y, z-rhs.z);
}
Vector Vector::operator + (const Vector& rhs) {
   return Vector(x+rhs.x, y+rhs.y, z+rhs.z);
}
/*
Vector Vector::operator * (const Vector a) {
   return Vector(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);
}*/
Vector Vector::operator * (const double rhs) {
   return Vector(x*rhs, y*rhs, z*rhs);
}
Vector Vector::operator * (const float rhs) {
   return Vector(x*rhs, y*rhs, z*rhs);
}
Vector Vector::operator * (const int rhs) {
   return Vector(x*rhs, y*rhs, z*rhs);
}
Vector Vector::operator / (const double rhs) {
   double inv = 1/rhs;
   // return Vector(x/rhs, y/rhs, z/rhs);
   return Vector(x*inv, y*inv, z*inv);
}
Vector Vector::operator / (const float rhs) {
   float inv = 1/rhs;
   // return Vector(x/rhs, y/rhs, z/rhs);
   return Vector(x*inv, y*inv, z*inv);
}
Vector Vector::operator / (const int rhs) {
   int inv = 1/rhs;
   // return Vector(x/rhs, y/rhs, z/rhs);
   return Vector(x*inv, y*inv, z*inv);
}
Vector Vector::cross(const Vector& a) {
   return Vector(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);
}
double Vector::mag2(){
   return x*x+y*y+z*z; 
}
// TODO: LOOK INTO OPTIMIZING THIS SQRT() FUNCTION
double Vector::mag(){
   return sqrt(x*x+y*y+z*z); 
}

double Vector::dot(const Vector& a) {
   return x * a.x + y * a.y + z * a.z;
}

double& Vector::operator[](int index) {
   if (index == 0) return x;
   if (index == 1) return y;
   if (index == 2) return z;
   throw std::out_of_range("Index out of range");
}

// fast inverse square root to approximate vector normalization
double fastInverseSqrt(double number) {
   long i;
   double x2, y;
   x2 = number * 0.5;
   y  = number;
   i  = *(long*)&y;
   i  = 0x5fe6eb50c7b537a9 - (i >> 1);
   y  = *(double*)&i;
   y  = y * (1.5 - (x2 * y * y));
   return y;
}

Vector Vector::normalize() {
   double magSq = x*x + y*y + z*z;
   if (magSq == 0) {
      return *this;
   }
   double invMag = fastInverseSqrt(magSq);
   return Vector(x * invMag, y * invMag, z * invMag);
}

Vector solveScalers(Vector& v1, Vector& v2, Vector& v3, Vector C){
   double denom = v1.z*v2.y*v3.x-v1.y*v2.z*v3.x-v1.z*v2.x*v3.y+v1.x*v2.z*v3.y+v1.y*v2.x*v3.z-v1.x*v2.y*v3.z;
   double inv_denom = 1.0 / denom;
   double a = C.z*v2.y*v3.x-C.y*v2.z*v3.x-C.z*v2.x*v3.y+C.x*v2.z*v3.y+C.y*v2.x*v3.z-C.x*v2.y*v3.z;
   double b = -C.z*v1.y*v3.x+C.y*v1.z*v3.x+C.z*v1.x*v3.y-C.x*v1.z*v3.y-C.y*v1.x*v3.z+C.x*v1.y*v3.z;
   double c = C.z*v1.y*v2.x-C.y*v1.z*v2.x-C.z*v1.x*v2.y+C.x*v1.z*v2.y+C.y*v1.x*v2.z-C.x*v1.y*v2.z;
   return Vector(a*inv_denom, b*inv_denom, c*inv_denom);
}

Ray::Ray(const Vector& po, const Vector& ve): point(po), vector(ve){}
