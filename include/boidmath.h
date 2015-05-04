#ifndef BOIDMATH_H
#define BOIDMATH_H

#include <math.h>
#include <ngl/Vec3.h>
#include <ngl/Vec2.h>
#include <vector>

class BoidMath
{
public:
  BoidMath();
  ~BoidMath();
  static float distance(ngl::Vec3 a, ngl::Vec3 b);
  static float distance(ngl::Vec2 a, ngl::Vec2 b);
  static bool lineSphereIntersect(ngl::Vec3 ahead, ngl::Vec3 a, ngl::Vec3 b, float radius);
  static bool boxIntersectsSphere(ngl::Vec3 bMin, ngl::Vec3 bMax, ngl::Vec3 C, float r);
  static float pointToPlane(ngl::Vec3 a, ngl::Vec3 b, ngl::Vec3 c, ngl::Vec3 d);
  static std::vector<ngl::Vec3> calcFOV(ngl::Vec3 a, ngl::Vec3 b, float rotX, float fovAngle);
  template<typename T>
  static T lerp(T a, T b, float t);
  static ngl::Vec3 cresentPoint(float u, float piv);
  static ngl::Vec3 hornPoint(float u, float v);
  static std::vector<ngl::Vec3> borromeanPoint(float u, float r1, float r2);
  static ngl::Vec3 bezierPoint(ngl::Vec3 start, ngl::Vec3 end, ngl::Vec3 ctrl, float t);
};

#endif // BOIDMATH_H
