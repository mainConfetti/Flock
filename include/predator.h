//----------------------------------------------------------------------------------------------------------------------
/// @file Predator.h
/// @author Alexander la Tourelle
/// @date 02/04/15
/// @class Predator
/// @brief A class to contain all relevant data and methods for a predator in
/// a flocking system. The class contains data such as position, velocity and prey and
/// methods that manipulate this data such as setPursiut(), setSteering() and move().
/// The predator focuses on one boid in the flock at a time.
//----------------------------------------------------------------------------------------------------------------------
#ifndef PREDATOR_H
#define PREDATOR_H

#include <ngl/Vec3.h>

class Boid;

class Predator
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief cotr
  /// @param [in] _pos the initial position
  /// @param [in] _mass the mass of the predator
  //----------------------------------------------------------------------------------------------------------------------
  Predator(ngl::Vec3 _pos, float _mass);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~Predator();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the prey target for the predator
  /// @param [in] _prey the boid that will be set to be the prey
  //----------------------------------------------------------------------------------------------------------------------
  void setPrey(Boid *_prey);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the pursiut force vector
  /// @param [in] _pos the position of the prey
  /// @param [in] _targetSpeed the speed of the prey used to predict
  /// furture positions
  //----------------------------------------------------------------------------------------------------------------------
  void setPursiut(ngl::Vec3 _pos, float targetSpeed);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the avoid force vector
  //----------------------------------------------------------------------------------------------------------------------
  void setAvoid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to clear the prey selection
  //----------------------------------------------------------------------------------------------------------------------
  void clearPrey();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the flee force vector away from the walls
  /// of the world
  //----------------------------------------------------------------------------------------------------------------------
  void fleeWalls();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the target vector based on all the steering
  /// forces: pursiut flee, goal, avoid.
  //----------------------------------------------------------------------------------------------------------------------
  void setTarget();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the steering vector towards the target
  //----------------------------------------------------------------------------------------------------------------------
  void setSteering();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set predator's speed
  /// @param [in] _speed the speed value to be set
  //----------------------------------------------------------------------------------------------------------------------
  inline void setSpeed(float _speed){m_speed = _speed;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the new position based on the current steering
  /// vector, current velocity and speed
  //----------------------------------------------------------------------------------------------------------------------
  void updatePosition();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate and set the boid's rotation based on its velocity
  //----------------------------------------------------------------------------------------------------------------------
  void setRotation();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to move the boid by combining all the steering methods and
  /// updatePosition
  //----------------------------------------------------------------------------------------------------------------------
  void move();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the position of a future or
  /// current collision
  /// @param [in] _pos the position of the obstacle to check
  /// @param [in] _rad the radius of th eobstacle to check
  //----------------------------------------------------------------------------------------------------------------------
  void findObstacle(ngl::Vec3 _pos, float _rad);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the x and y rotations of a boid
  /// @return returns a Vec3 of (m_pitch, m_yaw, 0)
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getRotation(){return ngl::Vec3(m_pitch, m_yaw,0);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to get the position of the boid
  /// @return returns m_position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getPosition(){return m_position;}
private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_position;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's velocity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_velocity;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's prey
  //----------------------------------------------------------------------------------------------------------------------
  Boid *m_prey;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the goal position based on predicting the prey's future position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_goal;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the avoid force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_avoid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the target vector that is the result of the weighted
  /// average of all the steering forces
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_target;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the flee force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_flee;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector from the current velocity to the target vector.
  /// This ensures a smooth flight and the speed of the turning is
  /// controlled by the mass of the predator
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_steering;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the position of an obstacle that is goign to be or has
  /// been collided with
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_collisionPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's maximum speed
  //----------------------------------------------------------------------------------------------------------------------
  float m_maxSpeed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's minimum speed
  //----------------------------------------------------------------------------------------------------------------------
  float m_minSpeed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predators maximum acceleration rate
  //----------------------------------------------------------------------------------------------------------------------
  float m_maxAccelerate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the avoid force weight
  //----------------------------------------------------------------------------------------------------------------------
  float m_avoidWeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's speed
  //----------------------------------------------------------------------------------------------------------------------
  float m_speed;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's mass
  //----------------------------------------------------------------------------------------------------------------------
  float m_mass;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the yaw (y-axis rotation) of the predator
  //----------------------------------------------------------------------------------------------------------------------
  float m_yaw;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the pitch (x-axis rotation) of the predator
  //----------------------------------------------------------------------------------------------------------------------
  float m_pitch;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator's previous distance to the prey
  /// used to determine whether it has flown plat the prey and
  /// therefore needs to deccelerate
  //----------------------------------------------------------------------------------------------------------------------
  float m_prevDistance;
};

#endif // PREDATOR_H
