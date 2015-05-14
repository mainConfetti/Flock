#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "Predator.h"
#include "Boid.h"
#include "BoidMath.h"

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
  m_maxSpeed=2.0f;
  m_speed=0.5f;
  m_mass=15;
  m_seeAhead =10;
  m_avoidWeight = 1000;
  m_hasLeader=false;
  m_isLeader=false;
  m_predator=0;
  m_boundRadius=5;
  m_collisionPos = 0;
  m_searchRad = 10;
  m_fov=false;
  m_tail=false;
  m_FOV=100;
  m_steer = false;
  m_separationDistance = 10;
  m_tailLength=50;
}

Boid::~Boid()
{

}


void Boid::setPos(float _x, float _y, float _z)
{
  m_position.set(_x,_y,_z);
}

void Boid::setNeighbour(Boid *boid)
{
  //if field of view mod is on, calculate the field of view planes and check whether
  //the boid is contained between them. If it is, add the boid to m_neighbours vector
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
  // if field of view mode is not on, then add the boid to m_neighbours vector
  else
  {
    m_neighbours.push_back(boid);
  }
}

void Boid::clearNeighbour()
{
  //clear the m_neighbours vector
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

void Boid::setPredator(Predator *_predator)
{
  m_predator=_predator;
}

void Boid::setLeader(Boid *_leader)
{
  m_leader=_leader;
  m_hasLeader=true;
}

void Boid::setFovAngle(int _angle)
{
  m_FOV=_angle;
}

void Boid::setSpeed(float _speed)
{
  // set m_setSpeed and m_speed
  m_setSpeed=_speed;
  m_speed=m_setSpeed;
}

void Boid::clearLeader()
{
  // if the boid is a leader, demote it and return
  // its radius and mass to normal
  if(m_isLeader==true)
  {
    m_isLeader=false;
    m_boundRadius=4;
    m_mass/=3;
  }
  // if the boid is not a leader then set the m_hasLeader
  // flag to false and remove m_leader
  else
  {
    m_hasLeader=false;
    m_leader=0;
  }
  // revert speed back to the speed that was set in case
  // boids haave accelerated or decelerated suring the time
  // that the leader owas active
  m_speed=m_setSpeed;
}

void Boid::setCentroid()
{
  // calculate the centroid of the boid's neighborus by
  // averaging their positions
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
  // cohesion is calculated as a vector towards the local centroid
  setCentroid();
  if(m_neighbours.size()>0)
  {
    m_cohesion = m_centroid-m_position;
    if(m_cohesion.length()!=0)
      m_cohesion.normalize();
  }
  // if the boid has no neighbours then has no cohesion force
  else m_cohesion.set(0,0,0);
}

void Boid::setAlign()
{
  // alignment force is calculated by averaging the boid's neighbours's velocities
  if(m_neighbours.size()>0)
  {
    for(int i=0;i<m_neighbours.size();++i)
    {
      m_align+=m_neighbours[i]->getVelocity();
    }
    m_align /= m_neighbours.size();
    if(m_align.length()!=0)
    {
      m_align.normalize();
    }
  }
  // if the boid has no neighbours it has no alignment force
  else
    m_align.set(0,0,0);
}

void Boid::setSeparation()
{
  // separation is calculated by taking a vector from each neighbours position to the boid's
  // position and averaging them all.
  m_separation.set(0,0,0);
  for(int i=0;i<m_neighbours.size();++i)
  {
    // if the neighbour is within the boid's separation radius then add a vector from the
    // neighbour to the boids position to the separation vector.
    if(BoidMath::distance(m_position, m_neighbours[i]->getPosition())>m_separationDistance)
    {
      ngl::Vec3 pos(m_neighbours[i]->getPosition());
      ngl::Vec3 target = (m_position-pos);
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
}

void Boid::setAvoid()
{
  // first check neighbours for collisions, obstacle collisions are
  // checked in World.cpp
  for(int i=0; i<m_neighbours.size();++i)
  {
    findObstacle(m_neighbours[i]->getPosition(), m_neighbours[i]->getRadius());

  }
  if(m_velocity.length()!=0)
  {
    m_velocity.normalize();
  }
  // if there is a collision, calculate the avoidance force as a vector
  // from the collision position to the ahead vector
  m_avoid=(0,0,0);
  ngl::Vec3 ahead(m_position + m_velocity * m_seeAhead * m_speed);
  if(m_collisionPos != 0)
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
  m_collisionPos = 0;
}

void Boid::setFlee(ngl::Vec3 _flee)
{
  // the flee force is calculated as a vector from the flee position
  // to the boid
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
  // if a boid raoms outside of the world bounds
  // it experiences a force to turn it around that
  // increases the further away it goes.
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
  // the the target as a position slightly behind the leader
  behind*=7;
  follow = m_leader->getPosition()+behind;
  m_follow=follow-m_position;
  float distance = m_follow.length();
  // increase speed with distance from leader
  m_speed=m_maxSpeed*(distance/20);
  // cap the maximum speed
  if(m_speed>m_setSpeed+0.4)
  {
    m_speed=m_setSpeed+0.4;
    if(m_speed > m_maxSpeed)
    {
      m_speed=m_maxSpeed;
    }
  }
  // if the boid is in front ofthe leader, get out of its way
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
  // scale the avoid force by the avoid force meight
  m_avoid*=m_avoidWeight;
  // if the boid is a leader
  if(m_isLeader==true)
  {
    // and if it is steerable, set the target by combining the flee force, avoid force and goal position
    if(m_steer==true)
    {
      m_target=m_flee+m_avoid+m_goal;
    }
    // otherwise set is as wander force + flee force + avoid force
    else
    {
      m_target=m_wander+m_flee+m_avoid;
    }
  }
  // if the boid is not a leader
  else
  {
    // scale the steering forces by their weights
    m_separation*=m_separationWeight;
    m_align*=m_alignWeight;
    m_cohesion*=m_cohesionWeight;
    // there is a predator set the flee force increasing with proximity
    if(m_predator!=0)
    {
      m_flee*=10000.0/BoidMath::distance(m_predator->getPosition(), m_position);
    }
    // if the boid has a leader set the follow leader force, scale it depending on the boid's distance from the leader
    if(m_hasLeader==true)
    {
      m_follow*=BoidMath::distance(m_position, m_leader->getPosition())*2;
      m_target+=m_follow;
    }
    // set the target by combining the weighted steering forces
    m_target+=m_separation+m_cohesion+m_align+m_avoid+m_flee;
  }
}

void Boid::setSteering()
{
  // set a steering vector from the current velocity to the target
  m_steering = m_target-m_velocity;
  if(m_steering.length()!=0)
    m_steering.normalize();
}

void Boid::updatePosition()
{
  // update the velocity by adding the steering vector divided by the mass
  m_velocity = m_velocity+(m_steering/m_mass);
  // scale it by the speed
  if(m_velocity.length()!=0)
  {
    m_velocity.normalize();
    m_velocity*=m_speed;
  }
  // add the velocity to the current position tyo update it
  m_position = m_position+m_velocity;
}


void Boid::setRotate()
{
  m_yaw = atan2(m_velocity.m_x,m_velocity.m_z)*180/M_PI+180;
  m_pitch = atan2(m_velocity.m_y,sqrt(m_velocity.m_x*m_velocity.m_x+m_velocity.m_z*m_velocity.m_z))*180/M_PI;
}


void Boid::move()
{
  // combines all the steering methods into one for easy use outside the class
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
    //if the boid has only 1 or fewer neighbours, double its search radius
    if(m_neighbours.size()<=1 && m_searchRad<350)
    {
      m_searchRad*=2;
    }
    //if the search radius is not less than of equal to 10 and the boid has 5 or more neighbours then half the search radius
    if(m_searchRad<=!10 && m_neighbours.size()>=5)
    {
      m_searchRad/=2;
    }
    setSeparation();
    setAlign();
    setCohesion();
  }
  setAvoid();
  if(m_predator!=0)
  {
    setFlee(m_predator->getPosition());
  }
  fleeWalls();
  // drives
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

void Boid::findObstacle(ngl::Vec3 _pos, float _rad)
{

  ngl::Vec3 ahead(m_position + m_velocity * m_seeAhead * m_speed);
  bool collision = BoidMath::collisionDetect(ahead, _pos, m_position, _rad);
  if(collision==true  && (m_collisionPos==0 || BoidMath::distance(m_position, _pos) < BoidMath::distance(m_position, m_collisionPos)))
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
  m_leader=0;
}

void Boid::setFOV(bool _fov)
{
  m_fov=_fov;
}

void Boid::setTail(bool _tail)
{
  if(_tail==false)
  {
    clearPrevPos();
  }
  m_tail=_tail;
}

void Boid::manageTail()
{
  m_prevPos.push_back(m_position-(m_velocity*3.5));
  while (m_prevPos.size()>m_tailLength)
  {
    m_prevPos.pop_front();
  }
}

void Boid::clearPrevPos()
{
  m_prevPos.clear();
}

void Boid::setSteer(bool _steer)
{
  m_steer = _steer;
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

