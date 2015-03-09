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
    void setId(int _id);
    int getId();
    void setNeighbour(Boid boid);
    void clearNeighbour();
    void getNeighbours();
    void setDistance(Boid boid);
    float getDistance();
    void calcCentroid();
    void calcCohesion();
    void Info();
    void setVelocity(float _x, float _y, float _z);
    void calcAlign();

private:
    ngl::Vec4 m_Position;
    std::vector<Boid> neighbours;
    int id;
    float distance;
    ngl::Vec3 m_Centroid;
    ngl::Vec3 m_Velocity;
    ngl::Vec3 m_Cohesion;
    ngl::Vec3 m_Align;


};

#endif // BOID_H
