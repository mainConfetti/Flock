#include "boid.h"
#include <math.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

const float g = 9.81;

Boid::Boid()
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    m_AlignWeight=1;
    m_SeparationWeight=2;
    m_CohesionWeight=2.5;
    m_Speed=0.4;
    m_Mass=8;
    MAX_SEE_AHEAD = 2.5;
    MAX_AVOID_FORCE = 2;
}

Boid::Boid(int _id)
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    setId(_id);
    m_AlignWeight=300;
    m_SeparationWeight=350;
    m_CohesionWeight=300;
    m_Speed=0.4;
    m_goalWeight=5;
    m_Mass=20;
    MAX_SEE_AHEAD =15;
    MAX_AVOID_FORCE = 500;
    hasLeader=false;
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
    ngl::Vec3 boidPos = (boid->getPosition().m_x, boid->getPosition().m_y, boid->getPosition().m_z);
    m_Distance = Distance3d(m_Position, boidPos);
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

void Boid::setGoal()
{
    if(hasLeader==true)
    {
        m_goal=m_leaderPos-m_Position;
        if(m_goal!=0)
            m_goal.normalize();
        m_goal*=m_goalWeight;
    }
    else if(m_Neighbours.size()<1)
    {
        m_goal=m_FlockCentroid-m_Position;
        if(m_goal!=0)
            m_goal.normalize();
        m_goal*=m_goalWeight;
    }
    else
        m_goal=0;
}

void Boid::setCentroid()
{
    m_Centroid = getPosition();
    if(m_Neighbours.size()>0)
    {
        for(int i=0;i<m_Neighbours.size();++i)
        {
            m_Centroid+=m_Neighbours[i]->getPosition();
        }
        m_Centroid /= m_Neighbours.size()+1;
    }
}

void Boid::setCohesion()
{
    if(m_Neighbours.size()>0)
    {
        m_Cohesion = m_Centroid-m_Position;
        if(m_Cohesion!=0)
            m_Cohesion.normalize();
        m_Cohesion*=m_CohesionWeight;
    }
    else m_Cohesion.set(0,0,0);
}

void Boid::setVelocity(float _x, float _y, float _z)
{
    m_Velocity.m_x=_x;
    m_Velocity.m_y=_y;
    m_Velocity.m_z=_z;
}

void Boid::setAlign()
{
    if(m_Neighbours.size()>0)
    {
        for(int i=0;i<m_Neighbours.size();++i)
        {
            m_Align+=m_Neighbours[i]->getVelocity();
        }
        m_Align /= m_Neighbours.size();
        if(m_Align!=0)
        m_Align.normalize();
        m_Align*=m_AlignWeight;
    }
    else
        m_Align.set(0,0,0);
}

void Boid::setSeparation()
{
    if(m_Neighbours.size()>0)
    {
        for(int i=0;i<m_Neighbours.size();++i)
        {
        ngl::Vec3 pos(m_Neighbours[i]->getPosition());
        m_Neighbours[i]->setDistance(this);
        float weight = (1.0/m_Neighbours[i]->getDistance());
        ngl::Vec3 target = (pos-m_Position)*weight;
        m_Separation += target;
        }
        m_Separation /= m_Neighbours.size();
        if(m_Separation!=0)
        m_Separation.normalize();
        m_Separation = -m_Separation;
        m_Separation*=m_SeparationWeight;
    }
    else
        m_Separation.set(0,0,0);
}

void Boid::setAvoid()
{
    ngl::Vec3 ahead;
    m_collisionPos = NULL;
    m_Velocity.normalize();
    ahead=m_Position + m_Velocity * MAX_SEE_AHEAD;
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
    if(ahead.m_x >= 100.0 || ahead.m_x <= -100.0)
    {
        m_avoid-=ahead*fabs(ahead.m_x/10);
    }
    if(ahead.m_y >= 100.0 || ahead.m_y <= -100.0)
    {
        m_avoid-=ahead*fabs(ahead.m_y/10);
    }
    if(ahead.m_z >= 100.0 || ahead.m_z <= -100.0)
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
    m_Target=m_Separation+m_Cohesion+m_Align+m_avoid+m_goal;
    if(m_Target.length()!=0)
        m_Target.normalize();
}

void Boid::setSteering()
{
    m_Steering = m_Target-m_Velocity;
    if(m_Steering.length()!=0)
        m_Steering.normalize();
}

void Boid::updatePosition()
{
//    ngl::Vec2 last(m_Position.m_x, m_Position.m_z);
//    if(prevPos.size()<2)
//            prevPos.push_back(last);
//    else
//    {
//        prevPos[0]=prevPos[1];
//        prevPos[1]=last;
//    }
    m_Velocity = m_Velocity+(m_Steering/m_Mass);
    m_Velocity.normalize();
    m_Position = m_Position+(m_Velocity*m_Speed);
}

void Boid::setFlockCentroid(ngl::Vec3 _flockCentroid)
{
     m_FlockCentroid.set(_flockCentroid);
}

float Boid::Distance3d(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

float Boid::Distance2d(ngl::Vec2 a, ngl::Vec2 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y)));
}

bool Boid::lineSphereIntersect(ngl::Vec3 ahead, ngl::Vec3 a, float radius)
{
    ngl::Vec3 ahead2=ahead*0.5;
    return Distance3d(a, ahead) <= radius || Distance3d(a, ahead2) <= radius || Distance3d(a, m_Position) <= radius;
}

void Boid::findObstacle(ngl::Vec3 ahead)
{
    m_collisionPos = NULL;
    for(int i=0;i<m_Neighbours.size();++i)
    {

        ngl::Vec3 obstaclePos(m_Neighbours[i]->getPosition());
        ngl::Vec3 temp = m_collisionPos;
        bool collision = lineSphereIntersect(ahead, obstaclePos, 10);
        if(collision==true  && (m_collisionPos==NULL || Distance3d(m_Position, obstaclePos) < Distance3d(m_Position, m_collisionPos)))
        {
            m_collisionPos=m_Neighbours[i]->getPosition();
        }
        else if(m_collisionPos!=NULL)
            m_collisionPos=temp;
    }
}



void Boid::setRotate()
{
    yaw = atan2(m_Velocity.m_x,m_Velocity.m_z)*180/M_PI+180;
    pitch = atan2(m_Velocity.m_y,sqrt(m_Velocity.m_x*m_Velocity.m_x+m_Velocity.m_z*m_Velocity.m_z))*180/M_PI;
    //ngl::Vec2 current(m_Position.m_x, m_Position.m_z);
    roll = 0;
//    if(prevPos.size()==2)
//    {
//        circlefrom3points(current, prevPos[0], prevPos[1]);
//        if(20>turnRadius>0.1)
//            roll = 90-(atan((m_Mass*g)/turnRadius)*180/M_PI);
//    }
}

ngl::Vec3 Boid::getRotation()
{
    return ngl::Vec3(pitch, yaw, 0);
}

void Boid::circlefrom3points(ngl::Vec2 A, ngl::Vec2 B, ngl::Vec2 C)
{
    // centre point (x, y) and radius r
    ngl::Vec2 centre;
    float r;

    //gradient of the line AB is mr and for BC it is mt
    float Mr = (B.m_y-A.m_y)/(B.m_x-A.m_x);
    float Mt = (C.m_y-B.m_y)/(C.m_x-B.m_x);

    // formula derived from equations for perpendicular
    // bisectors of chords AB and BC that intersect at the
    // centre
    centre.m_x = ((Mr*Mt*(C.m_y-A.m_y))+(Mr*(B.m_x+C.m_x))-(Mt*(A.m_x+B.m_x)))/(2*(Mr-Mt));
    // substitue x into equation for perpendicular bisector
    // of chord AB
    centre.m_y = (-1/Mr)*(centre.m_x-((A.m_x+B.m_x)/2))+((A.m_y+B.m_y)/2);
    // raidus is distance between and point A B C and the centre (x,y)
    turnRadius = Distance2d(A,centre);
}



















