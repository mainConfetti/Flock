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
    void setNeighbour(Boid *boid);
    void clearNeighbour();
    void getNeighbours();
    void setDistance(Boid * const boid);
    float getDistance();
    void calcCentroid();
    void calcCohesion();
    void Info();
    void setVelocity(float _x, float _y, float _z);
    void calcAlign();
    void calcSeparation();
    void setSWeight(int _separationWeight);
    void setCWeight(int _cohesionWeight);
    void setAWeight(int _alignWeight);
    void setMass(int _mass);
    int getSWeight();
    int getCWeight();
    int getAWeight();
    int getMass();
    void setTarget();
    void setSteering();
    void updatePosition();
    void setFlockCentroid(float _x, float _y, float _z);

private:
    ngl::Vec4 m_Position;
    std::vector<Boid *> m_Neighbours;
    int m_Id;
    float m_Distance;
    float m_Speed;
    int m_CohesionWeight;
    int m_AlignWeight;
    int m_SeparationWeight;
    int m_Mass;
    ngl::Vec3 m_Centroid;
    ngl::Vec3 m_Velocity;
    ngl::Vec3 m_Cohesion;
    ngl::Vec3 m_Align;
    ngl::Vec3 m_Separation;
    ngl::Vec3 m_Target;
    ngl::Vec3 m_Steering;
    ngl::Vec3 m_FlockCentroid;


};

#endif // BOID_H
