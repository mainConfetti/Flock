#include "obstacle.h"

Obstacle::Obstacle(float _radius, ngl::Vec3 _position)
{
  m_radius = _radius;
  m_position = _position;
  m_colour.set(0.0,1.0,0.8);
}

Obstacle::~Obstacle()
{

}

