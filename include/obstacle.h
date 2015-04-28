#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <ngl/Vec3.h>

class Obstacle
{
public:
    Obstacle(float _radius, ngl::Vec3 _position);
    ~Obstacle();
    ngl::Vec3 getPosition(){return m_position;}
    float getRadius(){return m_radius;}

private:
    float m_radius;
    ngl::Vec3 m_position;
};

#endif // OBSTACLE_H
