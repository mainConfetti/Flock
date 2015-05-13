#include "world.h"
#include "Boid.h"
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <BoidMath.h>

World::World(int _numBoids)
{
  for(int i=0; i < _numBoids; i++)
  {
    addBoid(200, 100, 50, 0.5, 15);
  }
  srand(time(NULL));
  m_predator=NULL;
  m_obstacles.clear();
  setCentroid();
  m_octree=new Octree(ngl::Vec3(0,0,0), 150, 6);
  std::cout << "created a flock of " << _numBoids << " boids!" << std::endl;
}

World::World(int _numBoids, int _cohesion, int _separation, int _alignment, float _speed, int _mass)
{
  for(int i=0; i < _numBoids; i++)
  {
    addBoid(_cohesion, _separation, _alignment, _speed, _mass);
  }
  srand(time(NULL));
  m_predator=NULL;
  m_obstacles.clear();
  setCentroid();
  m_octree=new Octree(ngl::Vec3(0,0,0), 150, 6);
  std::cout << "created a flock of " << _numBoids << " boids!" << std::endl;
}

World::~World()
{
  delete m_octree;
  delete m_predator;
}

void World::addBoid(int _cohesion, int _separation, int _alignment, float _speed, int _mass)
{
  float lower = -1.0, upper = 1.0;
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
  int id = m_flock.size() + 1;
  Boid boid(id);
  boid.setPos(0,0,0);
  boid.setCWeight(_cohesion);
  boid.setSWeight(_separation);
  boid.setAWeight(_alignment);
  boid.setSpeed(_speed);
  boid.setMass(_mass);
  boid.setVelocity(x,y,z);
  m_flock.push_back(boid);
}


void World::removeBoid()
{
  if(m_flock.size()>0)
  {
    m_flock.erase(m_flock.end()-1);
  }
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

void World::updateOctree()
{
  delete m_octree;
  m_octree=new Octree(ngl::Vec3(0,0,0), 150, 6);
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
  int rad = m_flock[x].getSearchRad();
  m_flock[x].clearNeighbour();
  m_octree->getPointsInsideSphere(centre, rad);
  for(int i=0;i<m_octree->m_resultsData.size();++i)
  {
    float id = (m_octree->m_resultsData[i].m_w);
    if(id!=m_flock[x].getId() && m_flock[x].isLeader()!=true)
    {
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
    setNeighboursOctree(i);
    for(int j=0;j<m_obstacles.size();++j)
    {
      m_flock[i].findObstacle(m_obstacles[j].getPosition(), m_obstacles[j].getRadius()+4);
    }
    for(int j=0;j<m_customObs.size();++j)
    {
      for(int k=0;k<m_customObs[j].size();++k)
      m_flock[i].findObstacle(m_customObs[j][k].getPosition(), m_customObs[j][k].getRadius()+4);
    }
    //std::cout<<"has "<<m_flock[i].getNeighbours()<<" neighbours"<<std::endl;
    m_flock[i].move();

  }
  if(m_predator!=NULL)
  {
    for(int j=0;j<m_obstacles.size();++j)
    {
      m_predator->findObstacle(m_obstacles[j].getPosition(), m_obstacles[j].getRadius());
    }
    m_predator->move();
  }
}


void World::addPredator()
{
  if(m_predator!=NULL)
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
  if(m_predator==NULL)
  {
    std::cout<<"there is no predator to remove"<<std::endl;
    return;
  }
  delete m_predator;
  m_predator=NULL;
  for(int i=0;i<m_flock.size();++i)
  {
    m_flock[i].setPredator(NULL);
  }
}

void World::setPrey()
{
  srand (time(NULL));
  int id = rand() % m_flock.size() + 1;
  m_predator->setPrey(&m_flock[id]);
}

void World::setLeader(int _id)
{
  for(int i=0;i<m_flock.size();++i)
  {
    m_flock[i].setLeader(&m_flock[_id]);
  }
  m_flock[_id].promoteToLeader();
}

void World::clearLeader()
{
  for(int i=0;i<m_flock.size();++i)
  {
    m_flock[i].clearLeader();
  }
}

void World::addObstacle()
{
  float lower = -100.0, upper = 100.0;
  int r;
  float fraction;
  r = rand();
  fraction = ((float) r / RAND_MAX) * (upper - lower);
  float x = lower + fraction;
  r=rand();
  fraction = ((float) r / RAND_MAX) * (upper - lower);
  float y = lower + fraction;
  r = rand();
  fraction = ((float) r / RAND_MAX) * (upper - lower);
  float z = lower + fraction;
  lower = 10;
  upper = 30.0;
  r = rand();
  fraction = ((float) r / RAND_MAX) * (upper - lower);
  float rad = lower + fraction;
  Obstacle obstacle(rad, ngl::Vec3(x,y,z), false);
  m_obstacles.push_back(obstacle);
}

void World::removeObstacle()
{
  if(m_obstacles.size()>0)
  {
    m_obstacles.erase(m_obstacles.end()-1);
  }
}

void World::clearObstacles()
{
  m_obstacles.clear();
  m_customObs.clear();
}

void World::createObstacles(int x)
{
  m_obstacles.clear();
  switch (x){
  case(0): clearObstacles();break;
  case(1):
  {
    for(float u=0.05; u<0.95; u+=0.05)
    {
      for(float v=0.05; v<0.95; v+=0.05)
      {
        ngl::Vec3 pos = BoidMath::cresentPoint(u,v);
        Obstacle obstacle(10, ngl::Vec3(pos.m_x*70-150, pos.m_y*70-150, pos.m_z*70), false);
        obstacle.setColour(ngl::Vec3(u,v,v));
        m_obstacles.push_back(obstacle);
      }
    }
  }break;
  case(2):
  {
    for(float u=0; u<1; u+=0.04)
    {
      for(float v=0; v<2*M_PI; v+=M_PI/6)
      {
        ngl::Vec3 pos = BoidMath::hornPoint(u,v);
        Obstacle obstacle((u+0.1)*10, ngl::Vec3(pos.m_x*30, pos.m_y*30-100, pos.m_z*30), false);
        obstacle.setColour(ngl::Vec3(2*M_PI/v, 2*M_PI/v,u));
        m_obstacles.push_back(obstacle);
      }
    }
  }break;
  case(3):
  {
    for(float u=0; u<2*M_PI; u+=M_PI/15)
    {
      std::vector<ngl::Vec3> pos = BoidMath::borromeanPoint(u, 2, 3);
      for(int i=0; i<3; ++i)
      {
        Obstacle obstacle(20, ngl::Vec3(pos[i].m_x*50,pos[i].m_y*50,pos[i].m_z*50), false);
        switch(i){
        case(0): obstacle.setColour(ngl::Vec3(1,0,0)); break;
        case(1): obstacle.setColour(ngl::Vec3(0,1,0)); break;
        case(2): obstacle.setColour(ngl::Vec3(0,0,1)); break;
        default: break;
        }
        m_obstacles.push_back(obstacle);
      }
    }
  } break;
  case(4):
  {
    for(int i=0; i<10; ++i)
    {
      ngl::Vec3 start(-150+(i*10),-150,150-(i*i));
      ngl::Vec3 end(-110+(i*20),-150,-120+(cos(i*4*M_PI/10)*20));
      ngl::Vec3 ctrl(50,300+(sin(i*2*M_PI/10)*30)-(i*15),20+(i*10));
      for(float t=0; t<=1.05; t+=0.05)
      {
        ngl::Vec3 pos = BoidMath::bezierPoint(start, end, ctrl, t);
        Obstacle obstacle(10, pos, false);
        m_obstacles.push_back(obstacle);
      }
    }
  } break;
  default : break;
  }
}

void World::drawOctree()
{
  m_octree->buildVAO();
  m_octree->draw();
}

void World::createCustomObstacle(ngl::Vec3 _start, ngl::Vec3 _end, ngl::Vec3 _ctrl, int _rad)
{
  std::vector<Obstacle> customObs;
  for(float t=0; t<=1.05; t+=0.05)
  {
    ngl::Vec3 pos = BoidMath::bezierPoint(_start, _end, _ctrl, t);
    Obstacle obstacle(_rad, pos, true);
    customObs.push_back(obstacle);
  }
  m_customObs.push_back(customObs);
}


void World::updateCustomObstacle(int _id, ngl::Vec3 _newStart, ngl::Vec3 _newEnd, ngl::Vec3 _newCtrl, int _newRad)
{
  int i=0;
  for(float t=0; t<=1.05; t+=0.05)
  {
    ngl::Vec3 pos = BoidMath::bezierPoint(_newStart, _newEnd, _newCtrl, t);
    Obstacle obstacle(_newRad, pos, true);
    m_customObs[_id][i] = obstacle;
    ++i;
  }
}

void World::removeCustomObstacle()
{
  m_customObs.pop_back();
}



