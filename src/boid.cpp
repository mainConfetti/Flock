#include "boid.h"

Boid::Boid()
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
}

Boid::Boid(int _id)
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setId(_id);
}

Boid::~Boid()
{

}


void Boid::setPos(float _x, float _y, float _z)
{
    m_Position.m_x = _x;
    m_Position.m_y = _y;
    m_Position.m_z = _z;
}

float Boid::getXPos()
{
    return m_Position.m_x;
}

float Boid::getYPos()
{
    return m_Position.m_y;
}

float Boid::getZPos()
{
    return m_Position.m_z;
}

void Boid::setId(int _id)
{
    id = _id;
}

int Boid::getId()
{
    return id;
}

void Boid::setDistance(Boid boid)
{
    float xd = m_Position.m_x - boid.getXPos();
    float yd = m_Position.m_y - boid.getYPos();
    float zd = m_Position.m_z - boid.getZPos();
    distance = sqrt(xd*xd + yd*yd + zd*zd);
}

float Boid::getDistance()
{
    return distance;
}

void Boid::setNeighbour(Boid boid)
{
    neighbours.push_back(boid);
}

void Boid::getNeighbours()
{
    std::cout<< "boid: " << id << " has neighbours: " << neighbours[0].getId() << ", " << neighbours[1].getId() << "and " << neighbours[2].getId() << std::endl;
}

void Boid::clearNeighbour()
{
    neighbours.clear();
}













