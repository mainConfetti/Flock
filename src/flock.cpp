#include "flock.h"
#include "boid.h"
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <time.h>

Flock::Flock()
{

}

Flock::Flock(int numBoids)
{
    for(int i=0; i < numBoids; i++)
    {
        addBoid();
    }
    srand(time(NULL));
    for(int i=0;i<flock.size();i++)
    {
        flock[i].setPos((rand()%10+1), (rand()%10+1), (rand()%10+1));
        std::cout << "boid" << flock[i].getId() << "at: (" << flock[i].getXPos() << "," << flock[i].getYPos() << "," << flock[i].getZPos() << ")" << std::endl;

    }

    std::cout << "created a flock of " << numBoids << " boids!" << std::endl;
}

Flock::~Flock()
{

}

void Flock::addBoid()
{
    int _id = flock.size() + 1;
    Boid boid(_id);
    flock.push_back(boid);
}


void Flock::printBoid(int i)
{
    if(i <= flock.size())
    {
        std::cout << "boid: " << flock[i].getId() << std::endl;
    }

}

int Flock::getSize()
{
    return flock.size();
}


void Flock::setNeighbours(int x)
{
    flock[x].clearNeighbour();

    for(int i=0; i < flock.size(); i++)
    {
        flock[i].setDistance(flock[x]);
    }
    for(int i=0; i < flock.size(); i++)
    {
        for(int k = 0; k < (flock.size()-1); k++)
        {
            if(flock[k].getDistance() > flock[k+1].getDistance())
            {
                Boid temp = flock[k];
                flock[k] = flock[k+1];
                flock[k+1] = temp;
            }
        }

    }
    flock[x].setNeighbour(flock[1]);
    flock[x].setNeighbour(flock[2]);
    flock[x].setNeighbour(flock[3]);
}

void Flock::queryNeighbours(int i)
{
    flock[i].getNeighbours();
}









