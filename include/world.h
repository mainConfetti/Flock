#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <cmath>
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <vector>
#include <boid.h>
#include <octree.h>
#include <predator.h>

class World
{
public:
    World();
    World(int numBoids);
    ~World();
    void addBoid();
    void setNeighbours(int i);
    void setNeighboursOctree(int x);
    void printBoid(int i);
    int getSize();
    void queryNeighbours(int i);
    void setCentroid();
    void updateWorld();
    void initNArray();
    void addPredator();
    void removePredator();
    void setPrey();
    void setLeader(int _id);
    void clearLeader();
    float distance3d(ngl::Vec3 a, ngl::Vec3);

    Predator *m_predator;
    std::vector<Boid> m_flock;
    ngl::Vec3 m_centroid;
private:

    // array used for setting boid neighbours
    std::vector<Boid*> m_nArray;
    void updateOctree();

    Octree *m_octree;

};

#endif // FLOCK_H
