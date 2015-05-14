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
  /// @param _predator the predator to set
  //----------------------------------------------------------------------------------------------------------------------
  void setPredator(Predator *_predator);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set a leader to the boid
  /// @param _leader the leader to set
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
  /// @brief method to set the tail length
  /// @param [in] _tailLength the value to be set
  //----------------------------------------------------------------------------------------------------------------------
  inline void setTailLength(int _tailLength){m_tailLength = _tailLength;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the separation distance
  /// @param [in] _sepDist the value to be set
  //----------------------------------------------------------------------------------------------------------------------
  inline void setSepDist(int _sepDist){m_separationDistance = _sepDist;}
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
  /// @param [in] _pos the position of the obstacle to check
  /// @param [in] _rad the radius of th eobstacle to check
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
  /// @brief the bounding radius of the boid
  //----------------------------------------------------------------------------------------------------------------------
  float m_boundRadius;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the boids position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_position;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector containing pointers to all the boid's neighbours
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Boid *> m_neighbours;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the boid's ID
  //----------------------------------------------------------------------------------------------------------------------
  int m_id;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the maximum speed the boid can achieve
  /// this is only relevant when the flock has a leader as boids that are
  /// further away from the leader will accelerate towards it
  //----------------------------------------------------------------------------------------------------------------------
  float m_maxSpeed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the boid's current speed
  //----------------------------------------------------------------------------------------------------------------------
  float m_speed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the initial speed set to the boid, this acts as a minimum
  //----------------------------------------------------------------------------------------------------------------------
  float m_setSpeed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the weight of the cohesion force acting on the boid
  //----------------------------------------------------------------------------------------------------------------------
  int m_cohesionWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the weight of the alignment force acting on the boid
  //----------------------------------------------------------------------------------------------------------------------
  int m_alignWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the weight of the separation force acting on the boid
  //----------------------------------------------------------------------------------------------------------------------
  int m_separationWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the distance at which boids begin to feel the separation force
  float m_separationDistance;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the mass of the boid, this effects how quickly it is able to
  /// change direction
  //----------------------------------------------------------------------------------------------------------------------
  int m_mass;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the distance the boid checks ahead for obstacles
  //----------------------------------------------------------------------------------------------------------------------
  float m_seeAhead;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the weight of the avoidance force acting on the boid
  /// this does not change and is permanently very high as avoiding
  /// obstacles has the highest priority
  //----------------------------------------------------------------------------------------------------------------------
  int m_avoidWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the averaged positions of the boid and its neighbours
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_centroid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the position of an obstacle that is goign to be or has
  /// been collided with
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_collisionPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the boids veclocity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_velocity;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the cohesion force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_cohesion;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the alignment force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_align;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the separation force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_separation;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the avoidance force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_avoid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the random target set to a wandering leader
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_wander;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the target vector that is the result of the weighted
  /// average of all the steering forces
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_target;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector from the current velocity to the target vector.
  /// This ensures a smooth flight and the speed of the turning is
  /// controlled by the mass of the boid
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_steering;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the flee force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_flee;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the follow leader force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_follow;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the goal force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_goal;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to a predator boid to avoid
  //----------------------------------------------------------------------------------------------------------------------
  Predator *m_predator;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the yaw (y-axis rotation) of the boid
  //----------------------------------------------------------------------------------------------------------------------
  float m_yaw;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the pitch (x-axis rotation) of the boid
  //----------------------------------------------------------------------------------------------------------------------
  float m_pitch;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the distance that the boid searches for neighbours
  //----------------------------------------------------------------------------------------------------------------------
  int m_searchRad;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the field of view angle
  //----------------------------------------------------------------------------------------------------------------------
  int m_FOV;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a timer that dictates how often the random wander
  /// target is set for a wandering leader
  //----------------------------------------------------------------------------------------------------------------------
  std::clock_t m_wanderTimer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a flag signalling whether the flock has a leader
  //----------------------------------------------------------------------------------------------------------------------
  bool m_hasLeader;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a flag signalling whether this boid is currently the leader
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isLeader;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer the the current leader of the flock
  //----------------------------------------------------------------------------------------------------------------------
  Boid* m_leader;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a flag signalling whether FOV mode is set on or off
  //----------------------------------------------------------------------------------------------------------------------
  bool m_fov;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a flag signalling whether boid trails are set on or off
  //----------------------------------------------------------------------------------------------------------------------
  bool m_tail;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the length of the boids trail. This is how many previous
  /// positions will be stored in m_prevPos
  //----------------------------------------------------------------------------------------------------------------------
  int m_tailLength;
  /// @brief a flag signalling whether leader steering is set on or off
  //----------------------------------------------------------------------------------------------------------------------
  bool m_steer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a deque housing the preivous 50 positions of the boid
  /// for drawing a trail behind it
  //----------------------------------------------------------------------------------------------------------------------
  std::deque<ngl::Vec3> m_prevPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to clear m_prevPos
  //----------------------------------------------------------------------------------------------------------------------
  void clearPrevPos();
  //----------------------------------------------------------------------------------------------------------------------
};

#endif // BOID_H
