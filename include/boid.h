#ifndef BOID_H
#define BOID_H

#include <stdlib.h>
#include <iostream>
#include <ngl/Vec3.h>
#include <vector>
#include <ngl/VertexArrayObject.h>
#include <ngl/Transformation.h>


class Boid
{
public:
    Boid();
    Boid(int _id);
    ~Boid();
    void setPos(float _x, float _y, float _z);
    ngl::Vec3 getPosition(){return m_Position;}
    ngl::Vec3 getVelocity(){return m_Velocity;}
    void setId(int _id);
    int getId();
    void setNeighbour(Boid *boid);
    void clearNeighbour();
    void getNeighbours();
    void setDistance(Boid *boid);
    float getDistance();
    void calcCentroid();
    void calcCohesion();
    void Info();
    void setVelocity(float _x, float _y, float _z);
    void calcAlign();
    void calcSeparation();
    void calcAvoid();
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
    void buildVAO();
    void draw();
    void setRotate();
    ngl::Transformation m_rotate;

private:
    ngl::Vec3 m_Position;
    std::vector<Boid *> m_Neighbours;
    int m_Id;
    float m_Distance;
    float m_Speed;
    int m_CohesionWeight;
    int m_AlignWeight;
    int m_SeparationWeight;
    int m_Mass;
    float MAX_SEE_AHEAD;
    int MAX_AVOID_FORCE;
    ngl::Vec3 m_Centroid;
    ngl::Vec3 m_collisionPos;
    ngl::Vec3 m_Velocity;
    ngl::Vec3 m_Cohesion;
    ngl::Vec3 m_Align;
    ngl::Vec3 m_Separation;
    ngl::Vec3 m_avoid;
    ngl::Vec3 m_Target;
    ngl::Vec3 m_Steering;
    ngl::Vec3 m_FlockCentroid;
    float Distance(ngl::Vec3 a, ngl::Vec3 b);
    bool lineSphereIntersect(ngl::Vec3 avoid, ngl::Vec3 a, float radius);
    void findObstacle(ngl::Vec3 ahead);
    ngl::VertexArrayObject *m_vao;
};

#endif // BOID_H
