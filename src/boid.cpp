#include "boid.h"

Boid::Boid()
{
    setPos(0.0, 0.0, 0.0);
    m_position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    m_align=1;
    m_separation=1;
    m_cohesion=1;
    m_speed=0.8;
    m_mass=10;
}

Boid::Boid(int _id)
{
    setPos(0.0, 0.0, 0.0);
    m_position.m_z = 0.0;
    setId(_id);
    m_align=1;
    m_separation=2;
    m_cohesion=2.2;
    m_speed=0.8;
    m_mass=10;
}

Boid::~Boid()
{

}


void Boid::setPos(float _x, float _y, float _z)
{
    m_position.m_x = _x;
    m_position.m_y = _y;
    m_position.m_z = _z;
}

float Boid::getXPos()
{
    return m_position.m_x;
}

float Boid::getYPos()
{
    return m_position.m_y;
}

float Boid::getZPos()
{
    return m_position.m_z;
}

float Boid::getXVel()
{
    return m_velocity.m_x;
}

float Boid::getYVel()
{
    return m_velocity.m_y;
}

float Boid::getZVel()
{
    return m_velocity.m_z;
}


void Boid::setId(int _id)
{
    m_id = _id;
}

int Boid::getId()
{
    return m_id;
}

void Boid::setDistance(Boid *const boid)
{
    float xd = m_position.m_x - boid->getXPos();
    float yd = m_position.m_y - boid->getYPos();
    float zd = m_position.m_z - boid->getZPos();
    m_distance = sqrt(xd*xd + yd*yd + zd*zd);
}

float Boid::getDistance()
{
    return m_distance;
}


void Boid::setVelocity(float _x, float _y, float _z)
{
    m_velocity.m_x=_x;
    m_velocity.m_y=_y;
    m_velocity.m_z=_z;
}



void Boid::Info()
{
    std::cout<<"Pos: "<<m_position.m_x<<", "<<m_position.m_y<<", "<<m_position.m_z<<std::endl;
    std::cout<<"Vel: "<<m_velocity.m_x<<", "<<m_velocity.m_y<<", "<<m_velocity.m_z<<std::endl;
    std::cout<<"weights: sep - "<<m_separation<<", coh - "<<m_cohesion<<", align - "<<m_align<<std::endl;
}

void Boid::setSeparation(float _separation)
{
    m_separation = _separation;
}

void Boid::setCohesion(float _cohesion)
{
    m_cohesion = _cohesion;
}

void Boid::setAlign(float _align)
{
    m_align = _align;
}

void Boid::setMass(float _mass)
{
    m_mass = _mass;
}

float Boid::getSeparation()
{
    return m_separation;
}

float Boid::getAlign()
{
    return m_align;
}

float Boid::getCohesion()
{
    return m_cohesion;
}

float Boid::getSpeed()
{
    return m_speed;
}

float Boid::getMass()
{
    return m_mass;
}

void Boid::updatePosition()
{
    m_position = m_position+(m_velocity*m_speed);
}

void Boid::setFlockCentroid(float _x, float _y, float _z)
 {
     m_flockCentroid.m_x = _x;
     m_flockCentroid.m_y = _y;
     m_flockCentroid.m_z = _z;
     m_flockCentroid.normalize();
 }








