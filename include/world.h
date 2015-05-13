#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <cmath>
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <vector>
#include <Boid.h>
#include <Octree.h>
#include <predator.h>
#include <obstacle.h>

class World
{
public:
  World(int _numBoids);
  World(int _numBoids, int _cohesion, int _separation, int _alignment, float _speed, int _mass);
  ~World();
  void addBoid(int _cohesion, int _separation, int _alignment, float _speed, int _mass);
  void removeBoid();
  void setNeighbours(int i);
  void setNeighboursOctree(int x);
  void printBoid(int i);
  int getSize();
  void queryNeighbours(int i);
  void setCentroid();
  void updateWorld();
  void initNArray();
  void addPredator();
  void removePredator();
  void setPrey();
  void setLeader(int _id);
  void clearLeader();
  void addObstacle();
  void removeObstacle();
  void clearObstacles();
  void testSetNeighbours();
  Predator *m_predator;
  std::vector<Boid> m_flock;
  ngl::Vec3 m_centroid;
  std::vector<Obstacle> m_obstacles;
  void createObstacles(int _x);
  void createCustomObstacle(ngl::Vec3 _start, ngl::Vec3 _end, ngl::Vec3 _ctrl, int _rad);
  void drawOctree();
  std::vector<std::vector<Obstacle>> m_customObs;
  void updateCustomObstacle(int _id, ngl::Vec3 _newStart, ngl::Vec3 _newEnd, ngl::Vec3 _newCtrl, int _newRad);
  void removeCustomObstacle();
private:

  // array used for setting boid neighbours
  std::vector<Boid*> m_nArray;
  void updateOctree();
  Octree *m_octree;


};

#endif // FLOCK_H
