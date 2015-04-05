#include "world.h"
#include "boid.h"
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <time.h>

World::World()
{

}

World::World(int numBoids)
{
    for(int i=0; i < numBoids; i++)
    {
        addBoid();
    }
    srand(time(NULL));
    m_predator=nullptr;
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
        //std::cout << "boid" << m_flock[i].getId() << "at: (" << m_flock[i].getXPos() << "," << m_flock[i].getYPos() << "," << m_flock[i].getZPos() << ")" << std::endl;
        m_flock[i].setVelocity((((float) rand()/RAND_MAX)), (((float) rand()/RAND_MAX)), (((float) rand()/RAND_MAX)));
        //m_flock[i].setPredator(m_predator);

    }
    setCentroid();
    m_octree=new Octree(ngl::Vec3(0,0,0), 120, 4);
    //setPrey();
    std::cout << "created a flock of " << numBoids << " boids!" << std::endl;
}

World::~World()
{

}

void World::addBoid()
{
    int _id = m_flock.size() + 1;
    Boid boid(_id);
    m_flock.push_back(boid);
}


void World::printBoid(int i)
{
    if(i <= m_flock.size())
    {
        std::cout << "boid: " << m_flock[i].getId() << std::endl;
    }

}

int World::getSize()
{
    return m_flock.size();
}

void World::initNArray()
{
    m_nArray.clear();
    for(int i=0;i<m_flock.size();++i)
        m_nArray.push_back(&m_flock[i]);
}

void World::setNeighbours(int x)
{
    initNArray();
    m_flock[x].clearNeighbour();

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
    for(int i=0;i<m_nArray.size();i++)
    {
        if(i<3 || m_nArray[i]->getDistance()<5)
        {
            m_flock[x].setNeighbour(m_nArray[i]);
        }
    }

}
void World::updateOctree()
{
    delete m_octree;
    m_octree=new Octree(ngl::Vec3(0,0,0), 120, 4);
    ngl::Vec4 dataPoint;
    for(int i=0;i<m_flock.size();++i)
    {
        dataPoint.set(m_flock[i].getPosition().m_x,m_flock[i].getPosition().m_y,m_flock[i].getPosition().m_z,m_flock[i].getId());
        m_octree->insert(dataPoint);
    }
}

void World::setNeighboursOctree(int x)
{
    ngl::Vec3 centre = (m_flock[x].getPosition());
    float r = 20.0;
    m_flock[x].clearNeighbour();
    m_octree->getPointsInsideSphere(centre, r);
    for(int i=0;i<m_octree->temp_data.size();++i)
    {
        if(m_octree->temp_data[i]!=NULL)
        {
            float id = (m_octree->temp_data[i].m_w);
            if(id!=m_flock[x].getId())
                m_flock[x].setNeighbour(&m_flock[(int)id-1]);
        }
    }
}

void World::queryNeighbours(int i)
{
    m_flock[i].getNeighbours();

    std::cout<<"m_nArray size: "<<m_nArray.size()<<std::endl;
}

void World::setCentroid()
{
    for(int i=0;i<m_flock.size();++i)
    {
        m_centroid+=m_flock[i].getPosition();
    }
    m_centroid /= m_flock.size();
}

void World::updateWorld()
{
    setCentroid();
    updateOctree();
    for(int i=0; i<m_flock.size();++i)
    {
        m_octree->clearResults();
        //std::cout<<"Boid: "<<m_flock[i].getId()<<"-----------------------"<<std::endl;
        //setNeighbours(i);
        setNeighboursOctree(i);
        //m_flock[i].getNeighbours();
        m_flock[i].setFlockCentroid(m_centroid);
        m_flock[i].move();

    }
    if(m_predator!=nullptr)
    {
        m_predator->move();
    }
}


void World::addPredator()
{
    if(m_predator!=nullptr)
    {
        std::cout<<"there is already a predator in the world"<<std::endl;
        return;
    }
    m_predator = new Predator(-m_centroid, 12.0);
    setPrey();
    for(int i=0;i<m_flock.size();++i)
    {
        m_flock[i].setPredator(m_predator);
    }
}

void World::removePredator()
{
    if(m_predator==nullptr)
    {
        std::cout<<"there is no predator to remove"<<std::endl;
        return;
    }
    delete m_predator;
    m_predator=nullptr;
    for(int i=0;i<m_flock.size();++i)
    {
        m_flock[i].setPredator(nullptr);
    }
}

void World::setPrey()
{
    srand (time(NULL));
    int id = rand() % m_flock.size() + 1;
    m_predator->setPrey(&m_flock[id]);
}

float World::distance3d(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

