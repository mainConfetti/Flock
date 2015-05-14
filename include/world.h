//----------------------------------------------------------------------------------------------------------------------
/// @file World.h
/// @author Alexander la Tourelle
/// @date 10/03/15
/// @class World
/// @brief A class to contain and manage all the elements of a flocking system.
/// The class is responsible for managing the neighbours of boids, obstacles and
/// to pass messages from the predator to the prey and visa versa. The class also
/// contains functions to preduce preset obstacle courses
//----------------------------------------------------------------------------------------------------------------------
#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <cmath>
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <vector>
#include <Boid.h>
#include <Octree.h>
#include <Predator.h>
#include <Obstacle.h>

class World
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor
  /// @param [in] _numBoids the number of boids in the flock
  //----------------------------------------------------------------------------------------------------------------------
  World(int _numBoids);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor to set custom steering force weights, speed and mass as well as the number of boids
  /// @param [in] _numBoids the number of boids in the flock
  /// @param [in] _cohesion the custom cohesion force weight to set
  /// @param [in] _separation the custom spearation force weight to set
  /// @param [in] _alignment the custom alignment force weight to set
  /// @param [in] _speed the custom speed value to be set
  /// @param [in] _mass the custom mass value to be set
  //----------------------------------------------------------------------------------------------------------------------
  World(int _numBoids, int _cohesion, int _separation, int _alignment, float _speed, int _mass);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~World();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to add a boid to the flock
  /// @param [in] _cohesion the cohesion force weight to set to the boid
  /// @param [in] _separation the spearation force weight to set to the boid
  /// @param [in] _alignment the alignment force weight to set to the boid
  /// @param [in] _speed the speed value to set to the boid
  /// @param [in] _mass the mass value to set to the boid
  //----------------------------------------------------------------------------------------------------------------------
  void addBoid(int _cohesion, int _separation, int _alignment, float _speed, int _mass);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to remove a boid from the flock
  //----------------------------------------------------------------------------------------------------------------------
  void removeBoid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to st the boid position
  /// @param [in] _id the id of the boid to set neighbours to
  //----------------------------------------------------------------------------------------------------------------------
  void setNeighbours(int _id);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to retuirn the size of the flock
  //----------------------------------------------------------------------------------------------------------------------
  int getSize();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the centroid of the flock
  //----------------------------------------------------------------------------------------------------------------------
  void setCentroid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to update the flock and predators, updates the neighbours,
  /// collision positions, positions of the boids and predators and creates a new
  /// octree based on these new positions.
  //----------------------------------------------------------------------------------------------------------------------
  void updateWorld();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to add a predator to the system
  //----------------------------------------------------------------------------------------------------------------------
  void addPredator();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to remove a predator from the system
  //----------------------------------------------------------------------------------------------------------------------
  void removePredator();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the predator's prey
  //----------------------------------------------------------------------------------------------------------------------
  void setPrey();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the leader of the flock
  //----------------------------------------------------------------------------------------------------------------------
  void setLeader(int _id);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to clear the leader from the flock
  //----------------------------------------------------------------------------------------------------------------------
  void clearLeader();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to add an obstacle to the system
  //----------------------------------------------------------------------------------------------------------------------
  void addObstacle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to remove the last obstacle that was
  /// created from the system
  //----------------------------------------------------------------------------------------------------------------------
  void removeObstacle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to remove all obstacles from the system
  //----------------------------------------------------------------------------------------------------------------------
  void clearObstacles();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the predator in the system
  //----------------------------------------------------------------------------------------------------------------------
  Predator *m_predator;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vecotr containing all the boids in the flock
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Boid> m_flock;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector containing all the default and preset obstacles in the system
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Obstacle> m_obstacles;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to create a custom obstacle course
  /// @param [in] _x a flag to decide which preset to create
  /// x = 0: clears all the obstacles
  /// x = 1: creates a course based on Paul Bourke's cresent equation
  /// x = 2: creates a course based on Paul Bourke's horn equation
  /// x = 3: creates a course based on Paul bourke's eliptical borromean rings equation
  /// x = 4: creates a wavey tunnel course made up of a series of undulating bezier curves
  //----------------------------------------------------------------------------------------------------------------------
  void createObstacles(int _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to create a custom obstacle from a bezier curve
  /// @param [in] _start the bezier curve start point
  /// @param [in] _end the bezier curve end point
  /// @param [in] _ctrl the bezier curve control point
  /// @param [in] _rad the obstacle radius
  //----------------------------------------------------------------------------------------------------------------------
  void createCustomObstacle(ngl::Vec3 _start, ngl::Vec3 _end, ngl::Vec3 _ctrl, int _rad);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to draw the octree
  //----------------------------------------------------------------------------------------------------------------------
  void drawOctree();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector containing all the custom obstacles (there is a maximum of 10)
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<std::vector<Obstacle> > m_customObs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to update a custom obstacle from a bezier curve
  /// @param [in] _id the id of the obstacle to update
  /// @param [in] _newStart the bezier curve;s new start point
  /// @param [in] _newEnd the bezier curve's new end point
  /// @param [in] _newCtrl the bezier curve's new control point
  /// @param [in] _newRad the obstacle's new radius
  //----------------------------------------------------------------------------------------------------------------------
  void updateCustomObstacle(int _id, ngl::Vec3 _newStart, ngl::Vec3 _newEnd, ngl::Vec3 _newCtrl, int _newRad);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to remove the last created custom obstacle from the system
  //----------------------------------------------------------------------------------------------------------------------
  void removeCustomObstacle();

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the flock centroid, used to calculate the initial
  /// predator location (the reverse of the flock centroid)
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_centroid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to update the octree. deletes the previous octree,
  /// creates a new one and populates it with the new data points
  //----------------------------------------------------------------------------------------------------------------------
  void updateOctree();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief an octree to manage the space and optimise neighbour selection
  //----------------------------------------------------------------------------------------------------------------------
  Octree *m_octree;
};

#endif // FLOCK_H
