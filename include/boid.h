#ifndef BOID_H
#define BOID_H

#include <stdlib.h>
#include <iostream>
#include <ngl/Vec3.h>
#include <vector>
#include <deque>
#include <ctime>
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
    void setLeader(Boid *_leader);
    void clearLeader();
    // accessors
    float getDistance() const {return m_distance;}
    int getNeighbours() const {return m_neighbours.size();}
    ngl::Vec3 getPosition() const {return m_position;}
    ngl::Vec3 getVelocity() const {return m_velocity;}
    bool isLeader() const {return m_isLeader;}
    float getRadius() const {return m_boundRadius;}
    int getId() const {return m_id;}
    ngl::Vec3 getRotation() const {return ngl::Vec3(m_pitch, m_yaw, 0);}
    int getSearchRad() const {return m_searchRad;}
    std::deque<ngl::Vec3> getPrevPos() const {return m_prevPos;}
    // steering
    void setCentroid();
    void setCohesion();
    void setAlign();
    void setSeparation();
    void setAvoid();
    void setFlee(ngl::Vec3 _flee);
    void fleeWalls();
    void setWander();
    void followLeader();
    void setTarget();
    void setSteering();
    // action
    void updatePosition();
    void move();
    void setRotate();
    // other
    void getInfo();
    void promoteToLeader();
    void toggleFOV();
    void toggleTail();
    void findObstacle(ngl::Vec3 _pos, float _rad);
    void manageTail();

private:
    float m_boundRadius;
    ngl::Vec3 m_position;
    std::vector<Boid *> m_neighbours;
    int m_id;
    float m_distance;
    float MAX_SPEED;
    float m_speed;
    int m_cohesionWeight;
    int m_alignWeight;
    int m_separationWeight;
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
    ngl::Vec3 m_wander;
    ngl::Vec3 m_target;
    ngl::Vec3 m_steering;
    ngl::Vec3 m_flockCentroid;
    ngl::Vec3 m_flee;
    ngl::Vec3 m_follow;
    Predator *m_predator;
    float m_yaw;
    float m_pitch;
    float m_roll;
    float m_turnRadius;
    int m_searchRad;
    int m_FOV;
    std::clock_t m_wanderTimer;
    bool m_hasLeader;
    bool m_isLeader;
    Boid* m_leader;
    bool m_fov;
    bool m_tail;
    std::deque<ngl::Vec3> m_prevPos;
    void clearPrevPos();

};

#endif // BOID_H
