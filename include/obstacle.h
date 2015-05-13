#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <ngl/Vec3.h>

class Obstacle
{
public:
  Obstacle(float _radius, ngl::Vec3 _position, bool _custom);
  ~Obstacle();
  ngl::Vec3 getPosition(){return m_position;}
  float getRadius(){return m_radius;}
  ngl::Vec3 getColour(){return m_colour;}
  void setColour(ngl::Vec3 _colour){m_colour = _colour;}

private:
  float m_radius;
  ngl::Vec3 m_position;
  ngl::Vec3 m_colour;
  bool m_custom;
};

#endif // OBSTACLE_H
