#include "boid.h"

Boid::Boid()
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    m_AlignWeight=1;
    m_SeparationWeight=2;
    m_CohesionWeight=2.5;
    m_Speed=0.5;
    m_Mass=10;
    MAX_SEE_AHEAD = 2.5;
    MAX_AVOID_FORCE = 2;
}

Boid::Boid(int _id)
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    setId(_id);
    m_AlignWeight=5;
    m_SeparationWeight=12;
    m_CohesionWeight=15;
    m_Speed=0.8;
    m_Mass=12;
    MAX_SEE_AHEAD =0;
    MAX_AVOID_FORCE = 100;
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
    ngl::Vec3 boidPos = (boid->getXPos(), boid->getYPos(), boid->getZPos());
    m_Distance = Distance(m_Position.toVec3(), boidPos);
}

float Boid::getDistance()
{
    return m_Distance;
}

void Boid::setNeighbour(Boid *boid)
{
    m_Neighbours.push_back(boid);
}

void Boid::clearNeighbour()
{
    m_Neighbours.clear();
}

void Boid::getNeighbours()
{
    std::cout<< "boid: " << m_Id << " has neighbours: ";
    for(int i=0;i<m_Neighbours.size();++i)
        std::cout<<m_Neighbours[i]->getId()<<", ";
    std::cout<<std::endl;
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
    m_Cohesion*=m_CohesionWeight;
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
    m_Align*=m_AlignWeight;
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
    m_Separation*=m_SeparationWeight;
}

void Boid::calcAvoid()
{
    ngl::Vec3 ahead;
    m_collisionPos = NULL;
    m_Velocity.normalize();
    ahead=m_Position.toVec3() + m_Velocity * MAX_SEE_AHEAD;
    findObstacle(ahead);
    m_avoid=(0,0,0);
    if(m_collisionPos != NULL)
    {
        m_avoid.m_x=(ahead.m_x-m_collisionPos.m_x); m_avoid.m_y=(ahead.m_y-m_collisionPos.m_y); m_avoid.m_z=(ahead.m_z-m_collisionPos.m_z);
        if(m_avoid.length()==0)
            m_avoid.m_x+=1;
        m_avoid.normalize();
    }
    else
    {
        m_avoid*=0;
    }
    if(ahead.m_x >= 50.0 || ahead.m_x <= -50.0)
    {
        m_avoid-=ahead*fabs(ahead.m_x/10);
    }
    if(ahead.m_y >= 50.0 || ahead.m_y <= -50.0)
    {
        m_avoid-=ahead*fabs(ahead.m_y/10);
    }
    if(ahead.m_z >= 50.0 || ahead.m_z <= -50.0)
    {
        m_avoid-=ahead*fabs(ahead.m_z/10);
    }
    m_avoid*=MAX_AVOID_FORCE;

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
    //m_Target=m_Separation+m_Cohesion+m_Align;
    m_Target=m_Separation+m_Cohesion+m_Align+m_avoid;
    m_Target.normalize();
}

void Boid::setSteering()
{
    m_Steering = m_Target-m_Velocity;
    m_Steering.normalize();
}

void Boid::updatePosition()
{
    m_Velocity = m_Velocity+(m_Steering/m_Mass);
    m_Velocity.normalize();
    m_Position = m_Position+(m_Velocity*m_Speed);
}

void Boid::setFlockCentroid(float _x, float _y, float _z)
 {
     m_FlockCentroid.m_x = _x;
     m_FlockCentroid.m_y = _y;
     m_FlockCentroid.m_z = _z;
     m_FlockCentroid.normalize();
 }

float Boid::Distance(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

bool Boid::lineSphereIntersect(ngl::Vec3 ahead, ngl::Vec3 a, float radius)
{
    ngl::Vec3 ahead2=ahead*0.5;
    return Distance(a, ahead) <= radius || Distance(a, ahead2) <= radius || Distance(a, m_Position.toVec3()) <= radius;
}

void Boid::findObstacle(ngl::Vec3 ahead)
{
    m_collisionPos = NULL;
    int m;
    for(int i=0;i<m_Neighbours.size();++i)
    {

        ngl::Vec3 obstaclePos;
        obstaclePos.m_x = m_Neighbours[i]->getXPos(); obstaclePos.m_y=m_Neighbours[i]->getYPos(); obstaclePos.m_z=m_Neighbours[i]->getZPos();
        bool collision = lineSphereIntersect(ahead, obstaclePos, 30);
        if(collision==1 && ((m_collisionPos=NULL)==true || Distance(m_Position.toVec3(), obstaclePos) < Distance(m_Position.toVec3(), m_collisionPos)))
        {
            m_collisionPos.m_x = m_Neighbours[i]->getXPos(); m_collisionPos.m_y=m_Neighbours[i]->getYPos(); m_collisionPos.m_z=m_Neighbours[i]->getZPos();
        }
    }
}



