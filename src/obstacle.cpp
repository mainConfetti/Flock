#include "obstacle.h"

Obstacle::Obstacle(float _radius, ngl::Vec3 _position, bool _custom)
{
  m_radius = _radius;
  m_position = _position;
  m_colour.set(0.0,0.5,0.8);
  m_custom = _custom;
}

Obstacle::~Obstacle()
{

}

