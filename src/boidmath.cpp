#include "boidmath.h"
#include <iostream>
#include <math.h>
#include <ngl/Transformation.h>

BoidMath::BoidMath()
{

}

BoidMath::~BoidMath()
{

}


float BoidMath::distance(ngl::Vec3 a, ngl::Vec3 b)
{
  return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

float BoidMath::distance(ngl::Vec2 a, ngl::Vec2 b)
{
  return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y)));
}

bool BoidMath::lineSphereIntersect(ngl::Vec3 ahead, ngl::Vec3 a, ngl::Vec3 b, float radius)
{
  ngl::Vec3 ahead2=ahead*0.5;
  return distance(a, ahead) <= radius || distance(a, ahead2) <= radius || distance(a, b) <= radius;
}

bool BoidMath::boxIntersectsSphere(ngl::Vec3 bMin, ngl::Vec3 bMax, ngl::Vec3 C, float r)
{
  float dist_squared = r * r;
  if (C.m_x < bMin.m_x)
    dist_squared -= (C.m_x - bMin.m_x)*(C.m_x - bMin.m_x);
  else if (C.m_x > bMax.m_x)
    dist_squared -= (C.m_x - bMax.m_x)*(C.m_x - bMax.m_x);
  if (C.m_y < bMin.m_y)
    dist_squared -= (C.m_y - bMin.m_y)*(C.m_y - bMin.m_y);
  else if (C.m_y > bMax.m_y)
    dist_squared -= (C.m_y - bMax.m_y)*(C.m_y - bMax.m_y);
  if (C.m_z < bMin.m_z)
    dist_squared -= (C.m_z - bMin.m_z)*(C.m_z - bMin.m_z);
  else if (C.m_z > bMax.m_z)
    dist_squared -= (C.m_z - bMax.m_z)*(C.m_z - bMax.m_z);
  return dist_squared > 0;
}

float BoidMath::pointToPlane(ngl::Vec3 a, ngl::Vec3 b, ngl::Vec3 c, ngl::Vec3 x)
{
  ngl::Vec3 AB, AC, N;
  float d, result;
  //calculate the vectors from a to b and a to c
  AB = b-a;
  AC = c-a;
  // calculate their cross product to find the normal vector and normalise it
  N.cross(AB,AC);
  N.normalize();
  // substitute in point a to find d
  d = a.m_x*N.m_x + a.m_y*N.m_y + a.m_z*N.m_z;
  // check if point x is above, bellow or on the line
  result = x.m_x*N.m_x + x.m_y*N.m_y + x.m_z*N.m_z - d;

  return result;
}

std::vector<ngl::Vec3> BoidMath::calcFOV(ngl::Vec3 a, ngl::Vec3 b, float rotX, float fovAngle)
{
  ngl::Vec3 a1, a2, b1;
  ngl::Transformation trans;
  std::vector<ngl::Vec3> points;
  float alpha = (180-fovAngle)/2;

  a1=a;
  trans.setRotation(ngl::Vec3(0,alpha,0));
  a1=trans.getMatrix()*a1;
  a1+=b;
  points.push_back(a1);

  trans.reset();

  b1.set(0,-1,0);
  //std::cout<<"1st b1 = ("<<b1.m_x<<", "<<b1.m_y<<", "<<b1.m_z<<")"<<std::endl;
  trans.setRotation(ngl::Vec3(rotX,0,0));
  b1=trans.getMatrix()*b1;
  //std::cout<<"2nd b1 = ("<<b1.m_x<<", "<<b1.m_y<<", "<<b1.m_z<<")"<<std::endl;
  b1.m_x+=b.m_x;
  b1.m_y+=b.m_y;
  b1.m_z+=b.m_z;
  //std::cout<<"3rd b1 = ("<<b1.m_x<<", "<<b1.m_y<<", "<<b1.m_z<<")"<<std::endl;
  points.push_back(b1);
  //std::cout<<"b = ("<<b.m_x<<", "<<b.m_y<<", "<<b.m_z<<")"<<std::endl;
  //std::cout<<"b1 = ("<<b1.m_x<<", "<<b1.m_y<<", "<<b1.m_z<<")"<<std::endl;
  trans.reset();

  a2=a;
  trans.setRotation(ngl::Vec3(0,180-alpha,0));
  a2=trans.getMatrix()*a2;
  a2+=b;
  points.push_back(a2);

  return points;
}

template<typename T>
T BoidMath::lerp(T a, T b, float t)
{
  return (a*(1-t))+(b*t);
}

ngl::Vec3 BoidMath::cresentPoint(float u, float v)
{
  ngl::Vec3 vertex;

  float piu = M_PI*u;
  float piv = M_PI*v;
  vertex.m_x = 2 + (sin(2*piu)*sin(2*piv)*sin(3*piv));
  vertex.m_y = 2 + (sin(2*piu)*sin(2*piv)*cos(3*piv));
  vertex.m_z = (cos(2*piu)*sin(2*piv)+(4*v)-2);
  return vertex;
}

ngl::Vec3 BoidMath::hornPoint(float u, float v)
{
  ngl::Vec3 vertex;

  float piu = M_PI*u;
  vertex.m_x = (2+(u*cos(v)))*(sin(2*piu));
  vertex.m_y = (2+(u*cos(v)))*(cos(2*piu))+(2*u);
  vertex.m_z = u*sin(v);
  return vertex;
}

std::vector<ngl::Vec3> BoidMath::borromeanPoint(float u, float r1, float r2)
{
  ngl::Vec3 vert1, vert2, vert3;
  std::vector<ngl::Vec3> results;
  vert1.m_x = 0;
  vert1.m_y = r1*cos(u);
  vert1.m_z = r2*sin(u);
  results.push_back(vert1);
  vert2.m_x = r2*cos(u);
  vert2.m_y = 0;
  vert2.m_z = r1*sin(u);
  results.push_back(vert2);
  vert3.m_x = r1*cos(u);
  vert3.m_y = r2*sin(u);
  vert3.m_z = 0;
  results.push_back(vert3);
  return results;
}

ngl::Vec3 BoidMath::bezierPoint(ngl::Vec3 start, ngl::Vec3 end, ngl::Vec3 ctrl, float t)
{
  ngl::Vec3 start2 = lerp(start, ctrl, t);
  ngl::Vec3 end2 = lerp(ctrl, end, t);
  ngl::Vec3 point = lerp(start2, end2, t);
  return point;
}










