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
    void setId(int _id);
    int getId();
    void setNeighbour(Boid boid);
    void clearNeighbour();
    void getNeighbours();
    void setDistance(Boid boid);
    float getDistance();

private:
    ngl::Vec4 m_Position;
    std::vector<Boid> neighbours;
    int id;
    float distance;

};

#endif // BOID_H
