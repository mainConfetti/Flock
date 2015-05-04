#include "boid.h"
#include <boidmath.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <predator.h>

const float g = 9.81;
const float SEPARATION_DISTANCE = 10;
int tailLength =50;

Boid::Boid(int _id)
{
  setPos(0.0, 0.0, 0.0);
  m_position.m_z = 0.0;
  m_velocity.set(1.0, 0.0, 0.0);
  m_flee.set(0,0,0);
  m_id=_id;
  m_alignWeight=50;
  m_separationWeight=100;
  m_cohesionWeight=200;
  MAX_SPEED=0.5f;
  m_speed=0.5f;
  m_mass=15;
  MAX_SEE_AHEAD =40;
  MAX_AVOID_FORCE = 10000;
  m_hasLeader=false;
  m_isLeader=false;
  m_predator=nullptr;
  m_boundRadius=5;
  m_collisionPos = NULL;
  m_searchRad = 10;
  m_fov=false;
  m_tail=false;
  m_FOV=100;
  m_steer = false;
}

Boid::~Boid()
{

}


void Boid::setPos(float _x, float _y, float _z)
{
  m_position.set(_x,_y,_z);
}

void Boid::setDistance(Boid *const boid)
{
  ngl::Vec3 boidPos = (boid->getPosition().m_x, boid->getPosition().m_y, boid->getPosition().m_z);
  m_distance = BoidMath::distance(m_position, boidPos);
}

void Boid::setNeighbour(Boid *boid)
{
  if(m_fov==true)
  {
    std::vector<ngl::Vec3> planePoints = BoidMath::calcFOV(m_velocity, m_position, m_pitch, m_yaw);
    int plane1 = BoidMath::pointToPlane(m_position, planePoints[0], planePoints[1], boid->getPosition());
    int plane2 = BoidMath::pointToPlane(m_position, planePoints[2], planePoints[1], boid->getPosition());
    if((plane1<0 && plane2>0) || (plane1>0 && plane2<0))
    {
      m_neighbours.push_back(boid);
    }
  }
  else
  {
    m_neighbours.push_back(boid);
  }
}

void Boid::clearNeighbour()
{
  m_neighbours.clear();
}

void Boid::setVelocity(float _x, float _y, float _z)
{
  m_velocity.set(_x,_y,_z);
}

void Boid::setSWeight(int _separationWeight)
{
  m_separationWeight = _separationWeight;
}

void Boid::setCWeight(int _cohesionWeight)
{
  m_cohesionWeight = _cohesionWeight;
}

void Boid::setAWeight(int _alignWeight)
{
  m_alignWeight = _alignWeight;
}

void Boid::setMass(int _mass)
{
  m_mass = _mass;
}

void Boid::setFlockCentroid(ngl::Vec3 _flockCentroid)
{
  m_flockCentroid.set(_flockCentroid);
}

void Boid::setPredator(Predator *_predator)
{
  m_predator=_predator;
}

void Boid::setLeader(Boid *_leader)
{
  m_leader=_leader;
  m_hasLeader=true;
}

void Boid::clearLeader()
{
  if(m_isLeader==true)
  {
    m_isLeader=false;
    m_boundRadius=4;
    m_mass/=3;
  }
  else
  {
    m_hasLeader=false;
    m_leader=nullptr;
  }
  m_speed=MAX_SPEED;
}

void Boid::setCentroid()
{
  m_centroid = getPosition();
  if(m_neighbours.size()>0)
  {
    for(int i=0;i<m_neighbours.size();++i)
    {
      m_centroid+=m_neighbours[i]->getPosition();
    }
    m_centroid /= m_neighbours.size()+1;
  }
}

void Boid::setCohesion()
{
  setCentroid();
  if(m_neighbours.size()>0)
  {
    m_cohesion = m_centroid-m_position;
    if(m_cohesion.length()!=0)
      m_cohesion.normalize();
  }
  else m_cohesion.set(0,0,0);
}

void Boid::setAlign()
{
  if(m_neighbours.size()>0)
  {
    for(int i=0;i<m_neighbours.size();++i)
    {
      m_align+=m_neighbours[i]->getVelocity();
    }
    m_align /= m_neighbours.size();
    if(m_align.length()!=0)
      m_align.normalize();
  }
  else
    m_align.set(0,0,0);
}

void Boid::setSeparation()
{
  m_separation.set(0,0,0);
  for(int i=0;i<m_neighbours.size();++i)
  {
    m_neighbours[i]->setDistance(this);
    if(m_neighbours[i]->getDistance()>SEPARATION_DISTANCE)
    {
      ngl::Vec3 pos(m_neighbours[i]->getPosition());
      ngl::Vec3 target = (pos-m_position);
      m_separation += target;
    }
  }
  if(m_neighbours.size()>0)
  {
    m_separation /= m_neighbours.size();
  }
  if(m_separation.length()!=0)
  {
    m_separation.normalize();
  }
  m_separation = -m_separation;
}

void Boid::setAvoid()
{
  for(int i=0; i<m_neighbours.size();++i)
  {
    findObstacle(m_neighbours[i]->getPosition(), m_neighbours[i]->getRadius());

  }
  m_velocity.normalize();
  m_avoid=(0,0,0);
  ngl::Vec3 ahead(m_position + m_velocity * MAX_SEE_AHEAD);
  if(m_collisionPos != NULL)
  {
    m_avoid=(ahead-m_collisionPos);
    if(m_avoid.length()!=0)
    {
      m_avoid.normalize();
    }
  }
  else
  {
    m_avoid*=0;
  }
  m_collisionPos = NULL;
}

void Boid::setFlee(ngl::Vec3 _flee)
{
  if(BoidMath::distance(_flee,m_position)<50)
  {
    m_flee=-(_flee-m_position);
    if(m_flee.length()!=0)
    {
      m_flee.normalize();
    }
  }
}

void Boid::fleeWalls()
{
  if(m_position.m_x<=-150 || m_position.m_x>=150)
  {
    m_flee.m_x-=(m_position.m_x/2);
  }
  if(m_position.m_y<=-150 || m_position.m_y>=150)
  {
    m_flee.m_y-=(m_position.m_y/2);
  }
  if(m_position.m_z<=-150 || m_position.m_z>=150)
  {
    m_flee.m_z-=(m_position.m_z/2);
  }
}



void Boid::setWander()
{
  // every 0.5 secods change the goal
  if(((std::clock()-m_wanderTimer)/CLOCKS_PER_SEC)>0.5)
  {
    // randomly set a goal
    float lower = -80.0, upper = 80.0;
    int r;
    float fraction;
    r = rand();
    fraction = ((float) r / RAND_MAX) * (upper - lower);
    float x = (lower + fraction);
    r = rand();
    fraction = ((float) r / RAND_MAX) * (upper - lower);
    float y = (lower + fraction);
    r = rand();
    fraction = ((float) r / RAND_MAX) * (upper - lower);
    float z = (lower + fraction);
    m_wander.set(x,y,z);
    // reset timer
    m_wanderTimer = std::clock();
  }
}

void Boid::followLeader()
{
  ngl::Vec3 behind, follow;
  m_follow.set(0,0,0);
  behind=-(m_leader->getPosition());
  if(behind!=0)
  {
    behind.normalize();
  }
  behind*=7;
  follow = m_leader->getPosition()+behind;
  m_follow=follow-m_position;
  float distance = m_follow.length();
  m_speed=MAX_SPEED*(distance/20);
  if(m_speed>0.7)
  {
    m_speed=0.7;
  }

  if(BoidMath::distance(m_leader->getVelocity(), m_position)<5 || BoidMath::distance(m_leader->getPosition(), m_position)<5)
  {
    setFlee(m_leader->getPosition());
  }
  if(m_follow.length()!=0)
  {
    m_follow.normalize();
  }
}

void Boid::setTarget()
{
  m_target.set(0,0,0);
  m_avoid*=MAX_AVOID_FORCE;
  if(m_isLeader==true)
  {
    if(m_steer==true)
    {
      m_target=m_flee+m_avoid+m_goal;
    }
    else
    {
      m_target=m_wander+m_flee+m_avoid;
    }
  }
  else
  {
    m_separation*=m_separationWeight;
    m_align*=m_alignWeight;
    m_cohesion*=m_cohesionWeight;

    if(m_predator!=nullptr)
    {
      m_flee*=10000.0/BoidMath::distance(m_predator->getPosition(), m_position);
    }
    if(m_hasLeader==true)
    {
      m_follow*=BoidMath::distance(m_position, m_leader->getPosition())/200;
      m_target+=m_follow;
      m_separation*=1.3;
    }
    m_target+=m_separation+m_cohesion+m_align+m_avoid+m_flee;
  }
}

void Boid::setSteering()
{
  m_steering = m_target-m_velocity;
  if(m_steering.length()!=0)
    m_steering.normalize();
}

void Boid::updatePosition()
{
  m_velocity = m_velocity+(m_steering/m_mass);
  if(m_velocity.length()!=0)
  {
    m_velocity.normalize();
    m_velocity*=m_speed;
  }
  m_position = m_position+m_velocity;
}


void Boid::setRotate()
{
  m_yaw = atan2(m_velocity.m_x,m_velocity.m_z)*180/M_PI+180;
  m_pitch = atan2(m_velocity.m_y,sqrt(m_velocity.m_x*m_velocity.m_x+m_velocity.m_z*m_velocity.m_z))*180/M_PI;
  m_roll = 0;
}


void Boid::move()
{
  m_flee.set(0,0,0);
  if(m_isLeader==true)
  {
    if(m_steer == false)
    {
      setWander();
    }
    else
    {

    }
  }

  else
  {
    //perceptions
    if(m_hasLeader==true)
    {
      followLeader();
    }
    if(m_neighbours.size()<=1 && m_searchRad<350)
    {
      m_searchRad*=2;
    }
    if(m_searchRad!=10 && m_neighbours.size()>=10)
    {
      m_searchRad/=2;
    }
    setSeparation();
    setAlign();
    setCohesion();
  }
  // drives
  setAvoid();
  if(m_predator!=nullptr)
  {
    setFlee(m_predator->getPosition());
  }
  fleeWalls();
  setTarget();
  setSteering();
  // action selection
  updatePosition();
  setRotate();

  if(m_tail==true)
  {
    manageTail();
  }
}


void Boid::getInfo()
{
  std::cout<<"Pos: "<<m_position.m_x<<", "<<m_position.m_y<<", "<<m_position.m_z<<std::endl;
  std::cout<<"Vel: "<<m_velocity.m_x<<", "<<m_velocity.m_y<<", "<<m_velocity.m_z<<std::endl;
  std::cout<<"cetroid: "<<m_centroid.m_x<<", "<<m_centroid.m_y<<", "<<m_centroid.m_z<<std::endl;
  std::cout<<"cohesion target: "<<m_cohesion.m_x<<", "<<m_cohesion.m_y<<", "<<m_cohesion.m_z<<" : "<<m_cohesion.length()<<std::endl;
  std::cout<<"align target: "<<m_align.m_x<<", "<<m_align.m_y<<", "<<m_align.m_z<<" : "<<m_align.length()<<std::endl;
  std::cout<<"separation target: "<<m_separation.m_x<<", "<<m_separation.m_y<<", "<<m_separation.m_z<<" : "<<m_separation.length()<<std::endl;
  std::cout<<"target vector: "<<m_target.m_x<<", "<<m_target.m_y<<", "<<m_target.m_z<<std::endl;
  std::cout<<"steering vector: "<<m_steering.m_x<<", "<<m_steering.m_y<<", "<<m_steering.m_z<<std::endl;
  std::cout<<"weights: sep - "<<m_separationWeight<<", coh - "<<m_cohesionWeight<<", align - "<<m_alignWeight<<std::endl;
}

void Boid::findObstacle(ngl::Vec3 _pos, float _rad)
{

  ngl::Vec3 ahead(m_position + m_velocity * MAX_SEE_AHEAD);
  bool collision = BoidMath::lineSphereIntersect(ahead, _pos, m_position, _rad);
  if(collision==true  && (m_collisionPos==NULL || BoidMath::distance(m_position, _pos) < BoidMath::distance(m_position, m_collisionPos)))
  {
    m_collisionPos=_pos;
  }
}

void Boid::promoteToLeader()
{
  m_isLeader=true;
  m_hasLeader=false;
  m_boundRadius=8;
  m_mass*=3;
  m_leader=nullptr;
}

void Boid::toggleFOV()
{
  if(m_fov==false)
  {
    m_fov=true;
  }
  else
  {
    m_fov=false;
  }
}

void Boid::toggleTail()
{
  if(m_tail==false)
  {
    m_tail=true;
  }
  else
  {
    m_tail=false;
    clearPrevPos();
  }
}

void Boid::manageTail()
{
  m_prevPos.push_back(m_position-(m_velocity*3.5));
  if(m_prevPos.size()>tailLength)
  {
    m_prevPos.pop_front();
  }
}

void Boid::clearPrevPos()
{
  m_prevPos.clear();
}

void Boid::toggleSteer()
{
  bool result = (m_steer == false ? true : false);
  m_steer = result;
}

void Boid::steerLeader(int _dir)
{
  ngl::Transformation trans;
  trans.setPosition(-m_position.m_x,-m_position.m_y,-m_position.m_z);
  trans.setRotation(-m_pitch, -m_yaw, 0);
  m_goal = m_velocity*2;
  m_goal = m_goal * trans.getMatrix();
  switch (_dir){
  case 0: m_goal.m_y += 10; break;
  case 1: m_goal.m_y -= 10; break;
  case 2: m_goal.m_x -= 10; break;
  case 3: m_goal.m_x += 10; break;
  }
  m_goal = m_goal * trans.getInverseMatrix();
}
