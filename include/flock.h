#ifndef FLOCK_H
#define FLOCK_H

#include <stdlib.h>
#include <cmath>
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <vector>
#include <boid.h>

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

    std::vector<Boid> flock;

};

#endif // FLOCK_H
