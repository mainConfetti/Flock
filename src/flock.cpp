#include <flock.h>
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
    for(int i=0;i<m_flock.size();++i)
    {
        float lower = -10.0, upper = 10.0;
        int r;
        float fraction;
        r = rand();
        fraction = ((float) r / RAND_MAX) * (upper - lower);
        float x = lower + fraction;
        r = rand();
        fraction = ((float) r / RAND_MAX) * (upper - lower);
        float y = lower + fraction;
        r = rand();
        fraction = ((float) r / RAND_MAX) * (upper - lower);
        float z = lower + fraction;
        m_flock[i].setPos(x, y, z);
        std::cout << "boid" << m_flock[i].getId() << "at: (" << m_flock[i].getXPos() << "," << m_flock[i].getYPos() << "," << m_flock[i].getZPos() << ")" << std::endl;
        m_flock[i].setVelocity((((float) rand()/RAND_MAX)), (((float) rand()/RAND_MAX)), (((float) rand()/RAND_MAX)));

    }
    setCentroid();

    std::cout << "created a flock of " << numBoids << " boids!" << std::endl;
}

Flock::~Flock()
{

}

void Flock::addBoid()
{
    int _id = m_flock.size() + 1;
    Boid boid(_id);
    m_flock.push_back(boid);
}


void Flock::printBoid(int i)
{
    if(i <= m_flock.size())
    {
        std::cout << "boid: " << m_flock[i].getId() << std::endl;
    }

}

int Flock::getSize()
{
    return m_flock.size();
}

void Flock::initNArray()
{
    m_nArray.clear();
    for(int i=0;i<m_flock.size();++i)
        m_nArray.push_back(&m_flock[i]);
}

void Flock::setNeighbours(int x)
{
    initNArray();
    m_steer->clearNeighbour();

    for(int i=0; i < m_flock.size(); i++)
    {
        m_flock[i].setDistance(&m_flock[x]);
    }
    for(int i=0;i<m_nArray.size();i++)
    {
        for(int k=0;k<(m_nArray.size()-1);k++)
        {
            if(m_nArray[k]->getDistance()>m_nArray[k+1]->getDistance())
            {
                Boid * temp = m_nArray[k];
                m_nArray[k] = m_nArray[k+1];
                m_nArray[k+1] = temp;
            }
        }

    }
    m_steer->setNeighbour(m_nArray[1]);
    m_steer->setNeighbour(m_nArray[2]);
    m_steer->setNeighbour(m_nArray[3]);
}

void Flock::queryNeighbours(int i)
{
    m_steer->getNeighbours();
}

void Flock::setCentroid()
{
    for(int i=0;i<m_flock.size();++i)
    {
        m_centroid.m_x +=  m_flock[i].getXPos();
        m_centroid.m_y += m_flock[i].getYPos();
        m_centroid.m_z += m_flock[i].getZPos();
    }
    m_centroid /= m_flock.size();
}

void Flock::updateFlock()
{
    setCentroid();
    for(int i=0; i<m_flock.size();++i)
    {
        m_steer->setBoid(&m_flock[i]);
        setNeighbours(i);
        m_flock[i].setFlockCentroid(m_centroid.m_x, m_centroid.m_y, m_centroid.m_z);
        m_steer->updateVelocity();
        m_flock[i].updatePosition();
    }
}









