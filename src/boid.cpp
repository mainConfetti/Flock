#include "boid.h"

Boid::Boid()
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
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

float Boid::getXVel()
{
    return m_Velocity.m_x;
}

float Boid::getYVel()
{
    return m_Velocity.m_y;
}

float Boid::getZVel()
{
    return m_Velocity.m_z;
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

void Boid::calcCentroid()
{
    m_Centroid.m_x = ((getXPos()+neighbours[0].getXPos()+neighbours[1].getXPos()+neighbours[2].getXPos())/4);
    m_Centroid.m_y = ((getYPos()+neighbours[0].getYPos()+neighbours[1].getYPos()+neighbours[2].getYPos())/4);
    m_Centroid.m_z = ((getZPos()+neighbours[0].getZPos()+neighbours[1].getZPos()+neighbours[2].getZPos())/4);

}

void Boid::calcCohesion()
{
    calcCentroid();
    m_Cohesion = m_Centroid-m_Position.toVec3();
    m_Cohesion.normalize();
}

void Boid::setVelocity(float _x, float _y, float _z)
{
    m_Velocity.m_x=_x;
    m_Velocity.m_y=_y;
    m_Velocity.m_z=_z;
}

void Boid::calcAlign()
{
    m_Align.m_x=(((neighbours[0].getXVel())+(neighbours[1].getXVel())+(neighbours[2].getXVel()))/3.0);
    m_Align.m_y=(((neighbours[0].getYVel())+(neighbours[1].getYVel())+(neighbours[2].getYVel()))/3.0);
    m_Align.m_z=(((neighbours[0].getZVel())+(neighbours[1].getZVel())+(neighbours[2].getZVel()))/3.0);
    m_Align.normalize();
}

void Boid::Info()
{
    std::cout<<"Pos: "<<m_Position.m_x<<", "<<m_Position.m_y<<", "<<m_Position.m_z<<std::endl;
    std::cout<<"Vel: "<<m_Velocity.m_x<<", "<<m_Velocity.m_y<<", "<<m_Velocity.m_z<<std::endl;
    std::cout<<"cetroid: "<<m_Centroid.m_x<<", "<<m_Centroid.m_y<<", "<<m_Centroid.m_z<<std::endl;
    std::cout<<"cohesion target: "<<m_Cohesion.m_x<<", "<<m_Cohesion.m_y<<", "<<m_Cohesion.m_z<<std::endl;
    std::cout<<"align target: "<<m_Align.m_x<<", "<<m_Align.m_y<<", "<<m_Align.m_z<<std::endl;

}










