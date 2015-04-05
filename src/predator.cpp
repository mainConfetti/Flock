#include <predator.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <ngl/Transformation.h>
#include <random>
#include <boid.h>

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
}

Predator::~Predator()
{

}

void Predator::setPrey(Boid *_prey)
{
    m_prey=_prey;
    prevDistance=distance3d(m_prey->getPosition(), m_position);
}

void Predator::clearPrey()
{
    m_prey = NULL;
}

void Predator::setPursiut(ngl::Vec3 _pos, float targetSpeed)
{
    float predict = distance3d(m_prey->getPosition(), m_position)/10;
    m_goal=(_pos+targetSpeed*predict)-m_position;
    if(m_goal.length()!=0)
    {
        m_goal.normalize();
    }
}

//void Predator::wander()
//{
//    // every 0.5 secods change the goal
//    if((std::clock()-wanderTimer)/CLOCKS_PER_SEC>0.5)
//    {
//        // randomly set a goal
//        float lower = -50.0, upper = 50.0;
//        int r;
//        float fraction;
//        r = rand();
//        fraction = ((float) r / RAND_MAX) * (upper - lower);
//        float x = (lower + fraction);
//        r = rand();
//        fraction = ((float) r / RAND_MAX) * (upper - lower);
//        float y = (lower + fraction);
//        r = rand();
//        fraction = ((float) r / RAND_MAX) * (upper - lower);
//        float z = (lower + fraction);
//        m_goal.set(x,y,z);
//        // reset timer
//        wanderTimer = std::clock();
//    }
//}

void Predator::setTarget()
{
    m_goal*=10;
    m_target=m_goal+m_flee;
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
    float distance=distance3d(m_prey->getPosition(),m_position);
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


    fleeWalls();
    setTarget();
    setSteering();
    updatePosition();
    setRotation();
}

float Predator::distance3d(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

