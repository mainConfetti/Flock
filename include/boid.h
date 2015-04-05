#ifndef BOID_H
#define BOID_H

#include <stdlib.h>
#include <iostream>
#include <ngl/Vec3.h>
#include <vector>
#include <ngl/VertexArrayObject.h>
#include <ngl/Transformation.h>

class Predator;

class Boid
{
public:
    Boid(int _id);
    ~Boid();
    // set
    void setPos(float _x, float _y, float _z);
    void setDistance(Boid *boid);
    void setNeighbour(Boid *boid);
    void clearNeighbour();
    void setVelocity(float _x, float _y, float _z);
    void setSWeight(int _separationWeight);
    void setCWeight(int _cohesionWeight);
    void setAWeight(int _alignWeight);
    void setMass(int _mass);
    void setFlockCentroid(ngl::Vec3 _flockCentroid);
    void setPredator(Predator *_predator);
    // accessors
    float getDistance();
    void getNeighbours();
    ngl::Vec3 getPosition(){return m_position;}
    ngl::Vec3 getVelocity(){return m_velocity;}
    int getId();
    ngl::Vec3 getRotation();
    // steering
    void setCentroid();
    void setCohesion();
    void setAlign();
    void setSeparation();
    void setAvoid();
    void setGoal(ngl::Vec3 _goal);
    void setFlee(ngl::Vec3 _flee);
    void fleeWalls();
    void setEvade();
    void setTarget();
    void setSteering();
    // action
    void updatePosition();
    void move();
    void setRotate();
    // other
    void getInfo();
    void circlefrom3points(ngl::Vec2 A, ngl::Vec2 B, ngl::Vec2 C);

private:
    ngl::Vec3 m_position;
    std::vector<Boid *> m_neighbours;
    int m_id;
    float m_distance;
    float MAX_SPEED;
    int m_cohesionWeight;
    int m_alignWeight;
    int m_separationWeight;
    int m_goalWeight;
    int m_mass;
    float MAX_SEE_AHEAD;
    int MAX_AVOID_FORCE;
    ngl::Vec3 m_centroid;
    ngl::Vec3 m_collisionPos;
    ngl::Vec3 m_velocity;
    ngl::Vec3 m_cohesion;
    ngl::Vec3 m_align;
    ngl::Vec3 m_separation;
    ngl::Vec3 m_avoid;
    ngl::Vec3 m_target;
    ngl::Vec3 m_steering;
    ngl::Vec3 m_flockCentroid;
    ngl::Vec3 m_goal;
    ngl::Vec3 m_flee;
    Predator *m_predator;
    float distance3d(ngl::Vec3 a, ngl::Vec3 b);
    float distance2d(ngl::Vec2 a, ngl::Vec2 b);
    bool lineSphereIntersect(ngl::Vec3 avoid, ngl::Vec3 a, float radius);
    void findObstacle(ngl::Vec3 ahead);
    float m_yaw;
    float m_pitch;
    float m_roll;
    float m_turnRadius;
    std::vector<ngl::Vec2> prevPos;
    bool hasLeader;
    ngl::Vec3 m_leaderPos;
};

#endif // BOID_H
