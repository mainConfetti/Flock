//----------------------------------------------------------------------------------------------------------------------
/// @file BoidMath.h
/// @author Alexander la Tourelle
/// @date 23/04/15
/// @class BoidMath
/// @brief A simple class to contain all relevant mathematical operations
/// for the flocking system so as to aid organisation and to prevent code
/// duplication.
//----------------------------------------------------------------------------------------------------------------------
#ifndef BOIDMATH_H
#define BOIDMATH_H

#include <math.h>
#include <ngl/Vec3.h>
#include <ngl/Vec2.h>
#include <ngl/Mat4.h>
#include <vector>
//----------------------------------------------------------------------------------------------------------------------
class BoidMath
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor
  //----------------------------------------------------------------------------------------------------------------------
  BoidMath();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor
  //----------------------------------------------------------------------------------------------------------------------
  ~BoidMath();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  static float distance(ngl::Vec3 a, ngl::Vec3 b);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  overlaoded function to calculate the distance between two 2d points
  /// @param [in] a a 2d point
  /// @param [in] b the 2d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  static float distance(ngl::Vec2 a, ngl::Vec2 b);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to detect and predict collisions based on two points and a radius
  /// @param [in] ahead vector pointing ahead of the object we are checking for collisions, used for prediction
  /// @param [in] a the position of the object we are checking
  /// @param [in] b the position of the potential obstacle
  /// @param [in] radius the radius of th epotential obstacle
  /// @return returns true of there will be or is a collision between a and b or flase if there is not.
  //----------------------------------------------------------------------------------------------------------------------
  static bool collisionDetect(ngl::Vec3 ahead, ngl::Vec3 a, ngl::Vec3 b, float radius);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief query if a box intersects with a sphere
  /// @param [in] bMin the minimum corner of the box
  /// @param [in] bMax the maximum corner of the box
  /// @param [in] C the centre of the sphere
  /// @param [in] r the radius of the circle
  /// @return true if box intersects with sphere, false if not
  //----------------------------------------------------------------------------------------------------------------------
  static bool boxIntersectsSphere(ngl::Vec3 bMin, ngl::Vec3 bMax, ngl::Vec3 C, float r);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to check whether a point is on, bellow or above a plane
  /// @param [in] a a 3d point used to define a plane
  /// @param [in] b a 3d point used to define a plane
  /// @param [in] c a 3d point used to define a plane
  /// @param [in] d the 3d point to check against plane formed by a,b,c
  /// @return returns a float, if < 0 then the point is bellow the plane, > 0 then it is above the plane
  /// if = 0 then the point lies on the plane
  //----------------------------------------------------------------------------------------------------------------------
  static float pointToPlane(ngl::Vec3 a, ngl::Vec3 b, ngl::Vec3 c, ngl::Vec3 d);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to calculate 3 poitns on a plane enclosing the field of view of a boid
  /// defined by an angle fovAngle
  /// @param [in] a the velocity of the boid
  /// @param [in] b the position of the boid
  /// @param [in] rotX the boid's rotation in the x axis
  /// @param [in] fovAngle the boid's field of view angle
  /// @return returns a std::vector of 3 3d points that lie on the plane
  //----------------------------------------------------------------------------------------------------------------------
  static std::vector<ngl::Vec3> calcFOV(ngl::Vec3 a, ngl::Vec3 b, float rotX, float fovAngle);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to linearly interpolate between 2 points
  /// @param [in] a the start point
  /// @param [in] b the end point
  /// @param [in] t parameter to set how far along the interpolation (between 0 and 1)
  /// @return returns a point at distance t between points a and b
  //----------------------------------------------------------------------------------------------------------------------
  template<typename T>
  static T lerp(T a, T b, float t);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to calculate points on a formation made up of cresents.
  /// Paule Bourke (2002) Available at: <http://paulbourke.net/geometry/cresent/>.
  /// @param [in] u angle in radians 0<=u<=1.
  /// @param [in] piv pi*v where v is an angle in radians 0<=v<=1.
  /// @return returns a point on the double cresent with the input u and v values.
  //----------------------------------------------------------------------------------------------------------------------
  static ngl::Vec3 cresentPoint(float u, float piv);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to calculate points on a horn formation.
  /// Paule Bourke (2002) Available at: <http://paulbourke.net/geometry/horn/>.
  /// @param [in] u angle in radians 0<=u<=1.
  /// @param [in] v angle in radians 0<=v<=1.
  /// @return returns a point on the horn with the input u and v values.
  //----------------------------------------------------------------------------------------------------------------------
  static ngl::Vec3 hornPoint(float u, float v);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to calculate points on a set of eliptical borromean rings.
  /// Paule Bourke (1997) Available at: <http://paulbourke.net/geometry/borromean/>.
  /// @param [in] u angle in radians 0<=u<=2pi.
  /// @param [in] r1 shortest radius.
  /// @param [in] r2 longest radius.
  /// @return returns  std::vector of points, each on one of the 3 borromean rings at angle u.
  //----------------------------------------------------------------------------------------------------------------------
  static std::vector<ngl::Vec3> borromeanPoint(float u, float r1, float r2);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to calculate points on a bezier curve
  /// @param [in] start the starting point of the curve.
  /// @param [in] end the end point of the curve.
  /// @param [in] ctrl the control point of the curve.
  /// @param [in] t the interval along the bezier curve.
  /// @return returns a point at distance t along a bezier curve.
  //----------------------------------------------------------------------------------------------------------------------
  static ngl::Vec3 bezierPoint(ngl::Vec3 start, ngl::Vec3 end, ngl::Vec3 ctrl, float t);
};

#endif // BOIDMATH_H
