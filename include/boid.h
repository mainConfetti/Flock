#ifndef BOID_H
#define BOID_H

#include <stdlib.h>
#include <iostream>
#include <ngl/Vec4.h>
#include <vector>

class Boid
{
public:
    Boid();
    Boid(int _id);
    ~Boid();
    void setPos(float _x, float _y, float _z);
    float getXPos();
    float getYPos();
    float getZPos();
    float getXVel();
    float getYVel();
    float getZVel();
    float getSeparation();
    float getAlign();
    float getCohesion();
    float getSpeed();
    float getMass();
    void setId(int _id);
    int getId();
    void setDistance(Boid * const boid);
    float getDistance();
    void calcCentroid();
    void Info();
    void setVelocity(float _x, float _y, float _z);
    void setSeparation(float _separation);
    void setCohesion(float _cohesion);
    void setAlign(float _align);
    void setMass(float _mass);
    void updatePosition();
    void setFlockCentroid(float _x, float _y, float _z);

private:
    ngl::Vec4 m_position;
    int m_id;
    float m_distance;
    float m_speed;
    float m_cohesion;
    float m_align;
    float m_separation;
    float m_mass;
    ngl::Vec3 m_velocity;
    ngl::Vec3 m_flockCentroid;


};

#endif // BOID_H
