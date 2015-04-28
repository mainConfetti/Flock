#include <predator.h>
#include <stdlib.h>
#include <boidmath.h>
#include <ctime>
#include <ngl/Transformation.h>
#include <random>
#include <boid.h>

const float MAX_SEE_AHEAD=20;

Predator::Predator(ngl::Vec3 _pos, float _mass)
{
    m_position=_pos;
    m_mass=_mass;
    m_velocity.set(0.8,0.3,0.2);
    MAX_SPEED=1.3;
    MIN_SPEED=0.6;
    MAX_ACCELERATE=0.01;
    m_speed = 0.3;
    m_prey=NULL;
    MAX_AVOID_FORCE=1000;
    m_collisionPos=NULL;
}

Predator::~Predator()
{

}

void Predator::setPrey(Boid *_prey)
{
    m_prey=_prey;
    prevDistance=BoidMath::distance(m_prey->getPosition(), m_position);
}

void Predator::clearPrey()
{
    m_prey = NULL;
}

void Predator::setPursiut(ngl::Vec3 _pos, float targetSpeed)
{
    float predict = BoidMath::distance(m_prey->getPosition(), m_position)/10;
    m_goal=(_pos+targetSpeed*predict)-m_position;
    if(m_goal.length()!=0)
    {
        m_goal.normalize();
    }
}

void Predator::setAvoid()
{
    m_velocity.normalize();
    m_avoid=(0,0,0);
    ngl::Vec3 ahead(m_position + m_velocity * MAX_SEE_AHEAD);
    if(m_collisionPos != NULL)
    {
        m_avoid=(ahead-m_collisionPos);
        if(m_avoid.length()!=0)
        {
            m_avoid.normalize();
        }
    }
    else
    {
        m_avoid*=0;
    }
    m_collisionPos = NULL;
}


void Predator::setTarget()
{
    m_goal*=10;
    m_avoid*=MAX_AVOID_FORCE;
    m_target=m_goal+m_flee+m_avoid;
    if(m_target.length()!=0)
    {
        m_target.normalize();
    }
}

void Predator::setSteering()
{
    m_steering = m_target-m_velocity;
    if(m_steering.length()!=0)
    {
        m_steering.normalize();
    }
}

void Predator::fleeWalls()
{
    if(m_position.m_x<=-100 || m_position.m_x>=100)
    {
        m_flee.m_x-=(m_position.m_x/2);
    }
    if(m_position.m_y<=-100 || m_position.m_y>=100)
    {
        m_flee.m_y-=(m_position.m_y/2);
    }
    if(m_position.m_z<=-100 || m_position.m_z>=100)
    {
        m_flee.m_z-=(m_position.m_z/2);
    }
}

void Predator::updatePosition()
{
    m_velocity = m_velocity+(m_steering/m_mass);
    if(m_speed>MAX_SPEED)
    {
        m_speed=MAX_SPEED;
    }
    else if(m_speed<MIN_SPEED)
    {
        m_speed=MIN_SPEED;
    }
    if(m_velocity.length()!=0)
    {
        m_velocity.normalize();
    }
    m_velocity*=m_speed;
    m_position = m_position+m_velocity;
}

void Predator::setRotation()
{
    m_yaw = atan2(m_velocity.m_x,m_velocity.m_z)*180/M_PI+180;
    m_pitch = atan2(m_velocity.m_y,sqrt(m_velocity.m_x*m_velocity.m_x+m_velocity.m_z*m_velocity.m_z))*180/M_PI;
    //ngl::Vec2 current(m_position.m_x, m_position.m_z);
    m_roll = 0;
}

void Predator::move()
{
    m_flee.set(0,0,0);
    float distance=BoidMath::distance(m_prey->getPosition(),m_position);
    float delta = prevDistance-distance;
    if(delta>=0)
    {
        m_speed+=MAX_ACCELERATE;
    }
    else
    {
        m_speed-=MAX_ACCELERATE;
    }
    setPursiut(m_prey->getPosition(), 0.8);
    setAvoid();
    fleeWalls();
    setTarget();
    setSteering();
    updatePosition();
    setRotation();
}

void Predator::findObstacle(ngl::Vec3 _pos, float _rad)
{

    ngl::Vec3 ahead(m_position + m_velocity * MAX_SEE_AHEAD);
    bool collision = BoidMath::lineSphereIntersect(ahead, _pos, m_position, _rad);
    if(collision==true  && (m_collisionPos==NULL || BoidMath::distance(m_position, _pos) < BoidMath::distance(m_position, m_collisionPos)))
    {
        m_collisionPos=_pos;
    }
}
