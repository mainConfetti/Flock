#ifndef FLOCK_H
#define FLOCK_H

#include <stdlib.h>
#include <cmath>
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <vector>
#include <boid.h>
#include <steering.h>

class Flock
{
public:
    Flock();
    Flock(int numBoids);
    ~Flock();
    void addBoid();
    void setNeighbours(int i);
    void printBoid(int i);
    int getSize();
    void queryNeighbours(int i);
    void setCentroid();
    void updateFlock();
    void initNArray();

    std::vector<Boid> m_flock;
    ngl::Vec3 m_centroid;
private:
    // array used for setting boid neighbours
    std::vector<Boid*> m_nArray;
    Steering *m_steer;

};

#endif // FLOCK_H
