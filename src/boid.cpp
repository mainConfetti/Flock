#include "boid.h"

Boid::Boid()
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    m_AlignWeight=1;
    m_SeparationWeight=2;
    m_CohesionWeight=2.5;
    m_Speed=0.8;
    m_Mass=10;
}

Boid::Boid(int _id)
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setId(_id);
    m_AlignWeight=1;
    m_SeparationWeight=2;
    m_CohesionWeight=2.5;
    m_Speed=0.8;
    m_Mass=10;
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
    m_Id = _id;
}

int Boid::getId()
{
    return m_Id;
}

void Boid::setDistance(Boid *const boid)
{
    float xd = m_Position.m_x - boid->getXPos();
    float yd = m_Position.m_y - boid->getYPos();
    float zd = m_Position.m_z - boid->getZPos();
    m_Distance = sqrt(xd*xd + yd*yd + zd*zd);
}

float Boid::getDistance()
{
    return m_Distance;
}

void Boid::setNeighbour(Boid *boid)
{
    m_Neighbours.push_back(boid);
    std::cout<<m_Neighbours.size()<<std::endl;
}

void Boid::clearNeighbour()
{
    m_Neighbours.clear();
}

void Boid::getNeighbours()
{
    std::cout<< "boid: " << m_Id << " has neighbours: " << m_Neighbours[0]->getId() << ", " << m_Neighbours[1]->getId() << "and " << m_Neighbours[2]->getId() << std::endl;
}


void Boid::calcCentroid()
{
    m_Centroid.m_x = ((getXPos()+m_Neighbours[0]->getXPos()+m_Neighbours[1]->getXPos()+m_Neighbours[2]->getXPos())/4);
    m_Centroid.m_y = ((getYPos()+m_Neighbours[0]->getYPos()+m_Neighbours[1]->getYPos()+m_Neighbours[2]->getYPos())/4);
    m_Centroid.m_z = ((getZPos()+m_Neighbours[0]->getZPos()+m_Neighbours[1]->getZPos()+m_Neighbours[2]->getZPos())/4);

}

void Boid::calcCohesion()
{
    m_Cohesion = m_Centroid-m_Position.toVec3();
    m_Cohesion.normalize();
    std::cout<<"calced cohesion"<<std::endl;
}

void Boid::setVelocity(float _x, float _y, float _z)
{
    m_Velocity.m_x=_x;
    m_Velocity.m_y=_y;
    m_Velocity.m_z=_z;
}

void Boid::calcAlign()
{
    m_Align.m_x=(((m_Neighbours[0]->getXVel())+(m_Neighbours[1]->getXVel())+(m_Neighbours[2]->getXVel()))/3.0);
    m_Align.m_y=(((m_Neighbours[0]->getYVel())+(m_Neighbours[1]->getYVel())+(m_Neighbours[2]->getYVel()))/3.0);
    m_Align.m_z=(((m_Neighbours[0]->getZVel())+(m_Neighbours[1]->getZVel())+(m_Neighbours[2]->getZVel()))/3.0);
    m_Align.normalize();
    std::cout<<"calced align"<<std::endl;
}

void Boid::calcSeparation()
{
    ngl::Vec3 pos1(m_Neighbours[0]->getXPos(), m_Neighbours[0]->getYPos(), m_Neighbours[0]->getZPos());
    ngl::Vec3 pos2(m_Neighbours[1]->getXPos(), m_Neighbours[1]->getYPos(), m_Neighbours[1]->getZPos());
    ngl::Vec3 pos3(m_Neighbours[2]->getXPos(), m_Neighbours[2]->getYPos(), m_Neighbours[2]->getZPos());
    m_Neighbours[0]->setDistance(this);
    m_Neighbours[1]->setDistance(this);
    m_Neighbours[2]->setDistance(this);
    float weight1 = (1.0/m_Neighbours[0]->getDistance());
    float weight2 = (1.0/m_Neighbours[1]->getDistance());
    float weight3 = (1.0/m_Neighbours[2]->getDistance());
    ngl::Vec3 target1 = (pos1-m_Position.toVec3())*weight1;
    ngl::Vec3 target2 = (pos2-m_Position.toVec3())*weight2;
    ngl::Vec3 target3 = (pos3-m_Position.toVec3())*weight3;
    m_Separation = (target1+target2+target3)/3.0;
    m_Separation.normalize();
    m_Separation = -m_Separation;
    std::cout<<"calced sep"<<std::endl;

}

void Boid::Info()
{
    std::cout<<"Pos: "<<m_Position.m_x<<", "<<m_Position.m_y<<", "<<m_Position.m_z<<std::endl;
    std::cout<<"Vel: "<<m_Velocity.m_x<<", "<<m_Velocity.m_y<<", "<<m_Velocity.m_z<<std::endl;
    std::cout<<"cetroid: "<<m_Centroid.m_x<<", "<<m_Centroid.m_y<<", "<<m_Centroid.m_z<<std::endl;
    std::cout<<"cohesion target: "<<m_Cohesion.m_x<<", "<<m_Cohesion.m_y<<", "<<m_Cohesion.m_z<<" : "<<m_Cohesion.length()<<std::endl;
    std::cout<<"align target: "<<m_Align.m_x<<", "<<m_Align.m_y<<", "<<m_Align.m_z<<" : "<<m_Align.length()<<std::endl;
    std::cout<<"separation target: "<<m_Separation.m_x<<", "<<m_Separation.m_y<<", "<<m_Separation.m_z<<" : "<<m_Separation.length()<<std::endl;
    std::cout<<"target vector: "<<m_Target.m_x<<", "<<m_Target.m_y<<", "<<m_Target.m_z<<std::endl;
    std::cout<<"steering vector: "<<m_Steering.m_x<<", "<<m_Steering.m_y<<", "<<m_Steering.m_z<<std::endl;
    std::cout<<"weights: sep - "<<m_SeparationWeight<<", coh - "<<m_CohesionWeight<<", align - "<<m_AlignWeight<<std::endl;
}

void Boid::setSWeight(int _separationWeight)
{
    m_SeparationWeight = _separationWeight;
}

void Boid::setCWeight(int _cohesionWeight)
{
    m_CohesionWeight = _cohesionWeight;
}

void Boid::setAWeight(int _alignWeight)
{
    m_AlignWeight = _alignWeight;
}

void Boid::setMass(int _mass)
{
    m_Mass = _mass;
}

int Boid::getSWeight()
{
   return m_SeparationWeight;
}

int Boid::getCWeight()
{
    return m_CohesionWeight;
}

int Boid::getAWeight()
{
    return m_AlignWeight;
}

int Boid::getMass()
{
    return m_Mass;
}

void Boid::setTarget()
{
    float dist = sqrt(m_Position.m_x*m_Position.m_x + m_Position.m_y*m_Position.m_y + m_Position.m_z*m_Position.m_z);
    if(m_Position.m_x<=50.0 || m_Position.m_y<=50.0 || m_Position.m_z<=50.0)
    {
        m_Target=(m_Separation*m_SeparationWeight)+(m_Cohesion*m_CohesionWeight)+(m_Align*m_AlignWeight);
        m_Target.normalize();
    }
    //if boid is outside 50x50x50 cube, add steering towards centre (oposite to velocity)
    else if(m_Position.m_x>50.0 || m_Position.m_y>50.0 || m_Position.m_z>50.0)
    {
        m_Target = (m_Separation*m_SeparationWeight)+(m_Cohesion*m_CohesionWeight)+(m_Align*m_AlignWeight)-(m_Velocity*dist);
        m_Target.normalize();
    }
}

void Boid::setSteering()
{
    m_Steering = m_Target-m_Velocity;
    m_Steering.normalize();
}

void Boid::updatePosition()
{
    m_Velocity = m_Velocity+(m_Steering/m_Mass);
    m_Position = m_Position+(m_Velocity*m_Speed);
}

void Boid::setFlockCentroid(float _x, float _y, float _z)
 {
     m_FlockCentroid.m_x = _x;
     m_FlockCentroid.m_y = _y;
     m_FlockCentroid.m_z = _z;
     m_FlockCentroid.normalize();
 }








