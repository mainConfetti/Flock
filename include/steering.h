#ifndef STEERING_H
#define STEERING_H

#include <boid.h>

class Steering
{
public:
    Steering();
    Steering(Boid *_boid);
    ~Steering();
    void clearNeighbour();
    void setNeighbour(Boid *boid);
    void getNeighbours();
    void updateVelocity();
    void setBoid(Boid *boid);
private:
    Boid *m_boid;
    ngl::Vec3 m_centroid;
    ngl::Vec3 m_cohesion;
    ngl::Vec3 m_align;
    ngl::Vec3 m_separation;
    ngl::Vec3 m_target;
    ngl::Vec3 m_desired;
    ngl::Vec3 m_steering;
    ngl::Vec3 m_velocity;
    ngl::Vec3 m_position;
    std::vector<Boid *> m_Neighbours;
    void calcCentroid();
    void calcCohesion();
    void calcAlign();
    void calcSeparation();
    void setTarget();
    void setSteering();

};

#endif // STEERING_H
