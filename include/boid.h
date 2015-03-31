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
    void setCentroid();
    void setCohesion();
    void Info();
    void setVelocity(float _x, float _y, float _z);
    void setAlign();
    void setSeparation();
    void setAvoid();
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
    void setFlockCentroid(ngl::Vec3 _flockCentroid);
    void setRotate();
    ngl::Vec3 getRotation();
    void circlefrom3points(ngl::Vec2 A, ngl::Vec2 B, ngl::Vec2 C);
    void setGoal();

private:
    ngl::Vec3 m_Position;
    std::vector<Boid *> m_Neighbours;
    int m_Id;
    float m_Distance;
    float m_Speed;
    int m_CohesionWeight;
    int m_AlignWeight;
    int m_SeparationWeight;
    int m_goalWeight;
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
    ngl::Vec3 m_goal;
    float Distance3d(ngl::Vec3 a, ngl::Vec3 b);
    float Distance2d(ngl::Vec2 a, ngl::Vec2 b);
    bool lineSphereIntersect(ngl::Vec3 avoid, ngl::Vec3 a, float radius);
    void findObstacle(ngl::Vec3 ahead);
    float yaw;
    float pitch;
    float roll;
    float turnRadius;
    std::vector<ngl::Vec2> prevPos;
    bool hasLeader;
    ngl::Vec3 m_leaderPos;
};

#endif // BOID_H
