#ifndef PREDATOR_H
#define PREDATOR_H

#include <ngl/Vec3.h>

class Boid;

class Predator
{
public:
    Predator(ngl::Vec3 _pos, float _mass);
    ~Predator();
    void setPrey(Boid *_prey);
    void setPursiut(ngl::Vec3 _pos, float targetSpeed);
    void wander();
    void clearPrey();
    void fleeWalls();
    void setTarget();
    void setPosition();
    void setSteering();
    void updatePosition();
    void setRotation();
    void move();
    ngl::Vec3 getRotation(){return ngl::Vec3(m_pitch, m_yaw, m_roll);}
    ngl::Vec3 getPosition(){return m_position;}
private:
    ngl::Vec3 m_position;
    ngl::Vec3 m_velocity;
    Boid *m_prey;
    ngl::Vec3 m_goal;
    ngl::Vec3 m_target;
    ngl::Vec3 m_flee;
    ngl::Vec3 m_steering;
    float MAX_SPEED;
    float MIN_SPEED;
    float MAX_ACCELERATE;
    float m_speed;
    float m_mass;
    float m_yaw;
    float m_pitch;
    float m_roll;
    float distance3d(ngl::Vec3 a, ngl::Vec3 b);
    float prevDistance;
};

#endif // PREDATOR_H
