#include "steering.h"

Steering::Steering()
{
    m_velocity.m_x = m_boid->getXVel();
    m_velocity.m_y = m_boid->getYVel();
    m_velocity.m_z = m_boid->getZVel();
    m_position.m_x = m_boid->getXPos();
    m_position.m_y = m_boid->getYPos();
    m_position.m_z = m_boid->getZPos();
}

Steering::Steering(Boid *_boid)
{
    m_boid = _boid;
    m_velocity.m_x = m_boid->getXVel();
    m_velocity.m_y = m_boid->getYVel();
    m_velocity.m_z = m_boid->getZVel();
    m_position.m_x = m_boid->getXPos();
    m_position.m_y = m_boid->getYPos();
    m_position.m_z = m_boid->getZPos();
}

Steering::~Steering()
{

}

void Steering::setNeighbour(Boid *boid)
{
    m_Neighbours.push_back(boid);
}

void Steering::clearNeighbour()
{
    m_Neighbours.clear();
}

void Steering::getNeighbours()
{
    std::cout<< "boid: " << m_boid->getId() << " has neighbours: " << m_Neighbours[0]->getId() << ", " << m_Neighbours[1]->getId() << "and " << m_Neighbours[2]->getId() << std::endl;
}

void Steering::calcCentroid()
{
    m_centroid.m_x = ((m_position.m_x+m_Neighbours[0]->getXPos()+m_Neighbours[1]->getXPos()+m_Neighbours[2]->getXPos())/4);
    m_centroid.m_y = ((m_position.m_y+m_Neighbours[0]->getYPos()+m_Neighbours[1]->getYPos()+m_Neighbours[2]->getYPos())/4);
    m_centroid.m_z = ((m_position.m_z+m_Neighbours[0]->getZPos()+m_Neighbours[1]->getZPos()+m_Neighbours[2]->getZPos())/4);
}

void Steering::calcCohesion()
{
    m_cohesion = m_centroid-m_position;
    m_cohesion.normalize();
}

void Steering::calcAlign()
{
    m_align.m_x=(((m_Neighbours[0]->getXVel())+(m_Neighbours[1]->getXVel())+(m_Neighbours[2]->getXVel()))/3.0);
    m_align.m_y=(((m_Neighbours[0]->getYVel())+(m_Neighbours[1]->getYVel())+(m_Neighbours[2]->getYVel()))/3.0);
    m_align.m_z=(((m_Neighbours[0]->getZVel())+(m_Neighbours[1]->getZVel())+(m_Neighbours[2]->getZVel()))/3.0);
    m_align.normalize();
}

void Steering::calcSeparation()
{
    ngl::Vec3 pos1(m_Neighbours[0]->getXPos(), m_Neighbours[0]->getYPos(), m_Neighbours[0]->getZPos());
    ngl::Vec3 pos2(m_Neighbours[1]->getXPos(), m_Neighbours[1]->getYPos(), m_Neighbours[1]->getZPos());
    ngl::Vec3 pos3(m_Neighbours[2]->getXPos(), m_Neighbours[2]->getYPos(), m_Neighbours[2]->getZPos());
    m_Neighbours[0]->setDistance(m_boid);
    m_Neighbours[1]->setDistance(m_boid);
    m_Neighbours[2]->setDistance(m_boid);
    float weight1 = (1.0/m_Neighbours[0]->getDistance());
    float weight2 = (1.0/m_Neighbours[1]->getDistance());
    float weight3 = (1.0/m_Neighbours[2]->getDistance());
    ngl::Vec3 target1 = (pos1-m_position)*weight1;
    ngl::Vec3 target2 = (pos2-m_position)*weight2;
    ngl::Vec3 target3 = (pos3-m_position)*weight3;
    m_separation = (target1+target2+target3)/3.0;
    m_separation.normalize();
    m_separation = -m_separation;

}

void Steering::setTarget()
{
    float dist = sqrt(m_position.m_x*m_position.m_x + m_position.m_y*m_position.m_y + m_position.m_z*m_position.m_z);
    if(m_position.m_x<=50.0 || m_position.m_y<=50.0 || m_position.m_z<=50.0)
    {
        m_target=(m_separation*m_boid->getSeparation())+(m_cohesion*m_boid->getCohesion())+(m_align*m_boid->getAlign());
        m_target.normalize();
    }
    //if boid is outside 50x50x50 cube, add steering towards centre (oposite to velocity)
    else if(m_position.m_x>50.0 || m_position.m_y>50.0 || m_position.m_z>50.0)
    {
        m_target=(m_separation*m_boid->getSeparation())+(m_cohesion*m_boid->getCohesion())+(m_align*m_boid->getAlign())-(m_velocity*dist);
        m_target.normalize();
    }
}

void Steering::setSteering()
{
    m_desired = m_target-m_position;
    m_desired.normalize();
    m_desired*=m_boid->getSpeed();
    m_steering = m_desired-m_velocity;
}

void Steering::updateVelocity()
{
    calcCentroid();
    calcCohesion();
    calcSeparation();
    calcAlign();
    setTarget();
    setSteering();
    m_steering/=m_boid->getMass();
    m_velocity+=m_steering;
    m_velocity.normalize();

}

void Steering::setBoid(Boid *boid)
{
    m_boid = boid;
}
