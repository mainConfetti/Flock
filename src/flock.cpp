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
    for(int i=0;i<m_Flock.size();++i)
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
        m_Flock[i].setPos(x, y, z);
        //std::cout << "boid" << m_Flock[i].getId() << "at: (" << m_Flock[i].getXPos() << "," << m_Flock[i].getYPos() << "," << m_Flock[i].getZPos() << ")" << std::endl;
        m_Flock[i].setVelocity((((float) rand()/RAND_MAX)), (((float) rand()/RAND_MAX)), (((float) rand()/RAND_MAX)));

    }
    setCentroid();
    m_octree=new Octree(ngl::Vec3(0,0,0), 30.0, 4);

    std::cout << "created a flock of " << numBoids << " boids!" << std::endl;
}

Flock::~Flock()
{

}

void Flock::addBoid()
{
    int _id = m_Flock.size() + 1;
    Boid boid(_id);
    m_Flock.push_back(boid);
}


void Flock::printBoid(int i)
{
    if(i <= m_Flock.size())
    {
        std::cout << "boid: " << m_Flock[i].getId() << std::endl;
    }

}

int Flock::getSize()
{
    return m_Flock.size();
}

void Flock::initNArray()
{
    m_NArray.clear();
    for(int i=0;i<m_Flock.size();++i)
        m_NArray.push_back(&m_Flock[i]);
}

void Flock::setNeighbours(int x)
{
    initNArray();
    m_Flock[x].clearNeighbour();

    for(int i=0; i < m_Flock.size(); i++)
    {
        m_Flock[i].setDistance(&m_Flock[x]);
    }
    for(int i=0;i<m_NArray.size();i++)
    {
        for(int k=0;k<(m_NArray.size()-1);k++)
        {
            if(m_NArray[k]->getDistance()>m_NArray[k+1]->getDistance())
            {
                Boid * temp = m_NArray[k];
                m_NArray[k] = m_NArray[k+1];
                m_NArray[k+1] = temp;
            }
        }

    }
    for(int i=0;i<m_NArray.size();i++)
    {
        if(i<3 || m_NArray[i]->getDistance()<5)
        {
            m_Flock[x].setNeighbour(m_NArray[i]);
        }
    }

}
void Flock::updateOctree()
{
    delete m_octree;
    m_octree=new Octree(ngl::Vec3(0,0,0), 120, 4);
    ngl::Vec4 dataPoint;
    for(int i=0;i<m_Flock.size();++i)
    {
        dataPoint.set(m_Flock[i].getPosition().m_x,m_Flock[i].getPosition().m_y,m_Flock[i].getPosition().m_z,m_Flock[i].getId());
        m_octree->insert(dataPoint);
    }
}

void Flock::setNeighboursOctree(int x)
{
    ngl::Vec3 centre = (m_Flock[x].getPosition());
    float r = 50.0;
    m_Flock[x].clearNeighbour();
    m_octree->getPointsInsideSphere(centre, r);
    for(int i=0;i<m_octree->temp_data.size();++i)
    {
        if(m_octree->temp_data[i]!=NULL)
        {
            float id = (m_octree->temp_data[i].m_w)-1;
            if(id!=m_Flock[x].getId())
                m_Flock[x].setNeighbour(&m_Flock[(int)id]);
        }
    }
}

void Flock::queryNeighbours(int i)
{
    m_Flock[i].getNeighbours();

    std::cout<<"m_NArray size: "<<m_NArray.size()<<std::endl;
}

void Flock::setCentroid()
{
    for(int i=0;i<m_Flock.size();++i)
    {
        m_Centroid+=m_Flock[i].getPosition();
    }
    m_Centroid /= m_Flock.size();
}

void Flock::updateFlock()
{
    setCentroid();
    updateOctree();
    for(int i=0; i<m_Flock.size();++i)
    {
        m_octree->clearResults();
        //std::cout<<"Boid: "<<m_Flock[i].getId()<<"-----------------------"<<std::endl;
        //setNeighbours(i);
        setNeighboursOctree(i);
        //m_Flock[i].getNeighbours();
        m_Flock[i].setFlockCentroid(m_Centroid);
        m_Flock[i].setGoal();
        m_Flock[i].setCentroid();
        m_Flock[i].setSeparation();
        m_Flock[i].setAlign();
        m_Flock[i].setCohesion();
        m_Flock[i].setAvoid();
        m_Flock[i].setTarget();
        m_Flock[i].setSteering();
        m_Flock[i].updatePosition();
        m_Flock[i].setRotate();
    }
}









