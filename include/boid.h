//----------------------------------------------------------------------------------------------------------------------
/// @file Boid.h
/// @author Alexander la Tourelle
/// @date 10/03/15
/// @class Boid
/// @brief A class to contain all relevant data and methods for a boid in
/// a flocking system. The class contains data such as position and velocity and
/// methods that manipulate this data such as setTarget(), setSteering() and move().
/// the methods combine to form a flocking behaviour as set out by Craig Reynolds (1986)
//----------------------------------------------------------------------------------------------------------------------
#ifndef BOID_H
#define BOID_H

#include <ctime>
#include <deque>
#include <iostream>
#include <ngl/VertexArrayObject.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <stdlib.h>
#include <vector>
//----------------------------------------------------------------------------------------------------------------------
//foreard declaration
class Predator;
//----------------------------------------------------------------------------------------------------------------------
class Boid
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor
  /// @param [in] _id boid ID
  //----------------------------------------------------------------------------------------------------------------------
  Boid(int _id);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~Boid();
  // set
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to st the boid position
  /// @param [in] _x the x value ot be set
  /// @param [in] _y the y value ot be set
  /// @param [in] _z the z value ot be set
  //----------------------------------------------------------------------------------------------------------------------
  void setPos(float _x, float _y, float _z);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set a neighbour to the boid
  /// @param [in] boid the neighbouring boid
  /// @return appends boid to m_neighbours
  //----------------------------------------------------------------------------------------------------------------------
  void setNeighbour(Boid *boid);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to cclear the boid's neighbours
  /// @return clears m_neighbours
  //----------------------------------------------------------------------------------------------------------------------
  void clearNeighbour();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the boid's velocity
  /// @param [in] _x the x value ot be set
  /// @param [in] _y the y value ot be set
  /// @param [in] _z the z value ot be set
  //----------------------------------------------------------------------------------------------------------------------
  void setVelocity(float _x, float _y, float _z);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the weight on the separation force
  /// @param [in] _separationWeight the weight value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setSWeight(int _separationWeight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the weight on the cohesion force
  /// @param [in] _cohesionWeight the weight value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setCWeight(int _cohesionWeight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the weight on the alignment force
  /// @param [in] _alignWeight the weight value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setAWeight(int _alignWeight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the mass of the boid
  /// @param [in] _mass the value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setMass(int _mass);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set a predator to the boid
  /// @param predator the predator to set
  //----------------------------------------------------------------------------------------------------------------------
  void setPredator(Predator *_predator);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set a leader to the boid
  /// @param leader the leader to set
  //----------------------------------------------------------------------------------------------------------------------
  void setLeader(Boid *_leader);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to clear the leader. if the boid has a leader, remove it.
  /// if the boid is a leader, demote it.
  //----------------------------------------------------------------------------------------------------------------------
  void clearLeader();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the steering flag on a leade boid. enable user imput
  /// to steer leader.
  /// @param [in] _steer the value to set to m_steer (true or false)
  //----------------------------------------------------------------------------------------------------------------------
  void setSteer(bool _steer);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set a field of view angle to the boid
  /// @param [in] _angle the fov angle value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setFovAngle(int _angle);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the fov flag for a boid
  /// @param _fov the value to set to m_fov (true or false)
  //----------------------------------------------------------------------------------------------------------------------
  void setFOV(bool _fov);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the fov flag for a boid
  /// @param _tail the value to set to m_tail (true or false)
  //----------------------------------------------------------------------------------------------------------------------
  void setTail(bool _tail);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the speed of the boid
  /// @param _speed the speed value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setSpeed(float _speed);
  //----------------------------------------------------------------------------------------------------------------------
  // accessors
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the number of neighbours the boid has
  /// @return returns the size of m_neighbours
  //----------------------------------------------------------------------------------------------------------------------
  int getNeighbours() const {return m_neighbours.size();}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the position of the boid
  /// @return returns m_position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getPosition() const {return m_position;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the velocity of the boid
  /// @return returns m_velocity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getVelocity() const {return m_velocity;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to check if a boid is the leader
  /// @return returns m_isLeader
  //----------------------------------------------------------------------------------------------------------------------
  bool isLeader() const {return m_isLeader;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the bouding radius of the boid
  /// @return returns m_boundRadius
  //----------------------------------------------------------------------------------------------------------------------
  float getRadius() const {return m_boundRadius;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the id of a boid
  /// @return returns m_id
  //----------------------------------------------------------------------------------------------------------------------
  int getId() const {return m_id;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the x and y rotations of a boid
  /// @return returns a Vec3 of (m_pitch, m_yaw, 0)
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getRotation() const {return ngl::Vec3(m_pitch, m_yaw, 0);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the distance a boid will look for neighbours
  /// @return returns m_searchRad
  //----------------------------------------------------------------------------------------------------------------------
  int getSearchRad() const {return m_searchRad;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the preious positions of the boid
  /// @return returns m_prevPos deque
  //----------------------------------------------------------------------------------------------------------------------
  std::deque<ngl::Vec3> getPrevPos() const {return m_prevPos;}
  //----------------------------------------------------------------------------------------------------------------------
  // steering
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the centroid of a boid and its neighbours
  //----------------------------------------------------------------------------------------------------------------------
  void setCentroid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the cohesion vector
  //----------------------------------------------------------------------------------------------------------------------
  void setCohesion();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the alignment force vector
  //----------------------------------------------------------------------------------------------------------------------
  void setAlign();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the separation force vector
  //----------------------------------------------------------------------------------------------------------------------
  void setSeparation();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the avoidance force vector
  //----------------------------------------------------------------------------------------------------------------------
  void setAvoid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the flee force vector
  /// @param [in] _flee the point to flee from
  //----------------------------------------------------------------------------------------------------------------------
  void setFlee(ngl::Vec3 _flee);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the flee force vector away from the walls
  /// of the world
  //----------------------------------------------------------------------------------------------------------------------
  void fleeWalls();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set a random target position to create
  /// a "wander" behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void setWander();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set follow leader force vector
  //----------------------------------------------------------------------------------------------------------------------
  void followLeader();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to modify the leader's goal position to steer it
  /// @param [in] _dir input to a switch statement with 4 possible inputs:
  /// _dir = 0: up button pressed results in positive y translation
  /// _dir =  1: down button pressed results in negative y translation
  /// _dir =  2: left button pressed results in negative x translation
  /// _dir =  3: right button pressed results in positive x translation
  //----------------------------------------------------------------------------------------------------------------------
  void steerLeader(int _dir);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the target vector based on all the steering
  /// forces: separation, cohesion, alignment, flee, follow, goal, avoid.
  //----------------------------------------------------------------------------------------------------------------------
  void setTarget();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the steering vector towards the target
  //----------------------------------------------------------------------------------------------------------------------
  void setSteering();
  //----------------------------------------------------------------------------------------------------------------------
  // action
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the new position based on the current steering
  /// vector, current velocity and speed
  //----------------------------------------------------------------------------------------------------------------------
  void updatePosition();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to move the boid by combining all the steering methods and
  /// updatePosition
  //----------------------------------------------------------------------------------------------------------------------
  void move();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the boid's rotation based on its velocity
  //----------------------------------------------------------------------------------------------------------------------
  void setRotate();
  //----------------------------------------------------------------------------------------------------------------------
  // other
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to promote a boid to be the Leader
  //----------------------------------------------------------------------------------------------------------------------
  void promoteToLeader();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the position of a future or
  /// current collision
  //----------------------------------------------------------------------------------------------------------------------
  void findObstacle(ngl::Vec3 _pos, float _rad);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to manage the previous position deque
  /// to draw trails behind the boids.
  //----------------------------------------------------------------------------------------------------------------------
  void manageTail();
  //----------------------------------------------------------------------------------------------------------------------
private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_boundRadius;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_position;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Boid *> m_neighbours;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int m_id;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_distance;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float MAX_SPEED;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_speed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_setSpeed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int m_cohesionWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int m_alignWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int m_separationWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int m_mass;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float MAX_SEE_AHEAD;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int MAX_AVOID_FORCE;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_centroid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_collisionPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_velocity;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_cohesion;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_align;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_separation;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_avoid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_wander;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_target;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_steering;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_flockCentroid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_flee;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_follow;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_goal;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  Predator *m_predator;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_yaw;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_pitch;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_roll;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  float m_turnRadius;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int m_searchRad;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  int m_FOV;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  std::clock_t m_wanderTimer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  bool m_hasLeader;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isLeader;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  Boid* m_leader;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  bool m_fov;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  bool m_tail;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  bool m_steer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  std::deque<ngl::Vec3> m_prevPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the distance between two 3d points
  /// @param [in] a a 3d point
  /// @param [in] b the 3d point whos distance from a is to be calculated
  /// @return returns the distance of b from a
  //----------------------------------------------------------------------------------------------------------------------
  void clearPrevPos();
  //----------------------------------------------------------------------------------------------------------------------
};

#endif // BOID_H
