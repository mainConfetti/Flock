#include <boid.h>
#include <math.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <predator.h>

const float g = 9.81;
const float SEPARATION_DISTANCE = 5;

Boid::Boid(int _id)
{
    setPos(0.0, 0.0, 0.0);
    m_position.m_z = 0.0;
    m_velocity.set(1.0, 0.0, 0.0);
    m_flee.set(0,0,0);
    m_id=_id;
    m_alignWeight=12;
    m_separationWeight=14;
    m_cohesionWeight=8;
    MAX_SPEED=0.4f;
    m_goalWeight=1;
    m_mass=12;
    MAX_SEE_AHEAD =5;
    MAX_AVOID_FORCE = 3;
    hasLeader=false;
    m_predator=nullptr;
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
    m_distance = distance3d(m_position, boidPos);
}

void Boid::setNeighbour(Boid *boid)
{
    m_neighbours.push_back(boid);
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

float Boid::getDistance()
{
    return m_distance;
}


void Boid::getNeighbours()
{
    std::cout<< "boid: " << m_id << " has neighbours: ";
    for(int i=0;i<m_neighbours.size();++i)
        std::cout<<m_neighbours[i]->getId()<<", ";
    std::cout<<std::endl;
}

int Boid::getId()
{
    return m_id;
}

ngl::Vec3 Boid::getRotation()
{
    return ngl::Vec3(m_pitch, m_yaw, 0);
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
        if(m_cohesion!=0)
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
        if(m_align!=0)
            m_align.normalize();
    }
    else
        m_align.set(0,0,0);
}

void Boid::setSeparation()
{
    int inc=0;
    m_separation.set(0,0,0);
    for(int i=0;i<m_neighbours.size();++i)
    {
        m_neighbours[i]->setDistance(this);
        if(m_neighbours[i]->getDistance()>SEPARATION_DISTANCE)
        {
            ngl::Vec3 pos(m_neighbours[i]->getPosition());
            float weight = (1.0/m_neighbours[i]->getDistance());
            ngl::Vec3 target = (pos-m_position)*weight;
            m_separation += target;
            ++inc;
        }
    }
    if(inc>0)
    {
        m_separation /= inc;
    }
    if(m_separation!=0)
        m_separation.normalize();
    m_separation = -m_separation;
}

void Boid::setAvoid()
{
    ngl::Vec3 ahead;
    m_collisionPos = NULL;
    m_velocity.normalize();
    ahead=m_position + m_velocity * MAX_SEE_AHEAD;
    findObstacle(ahead);
    m_avoid=(0,0,0);
    if(m_collisionPos != NULL)
    {
        m_avoid=(m_position-m_collisionPos);
        if(m_avoid.length()!=0)
            m_avoid.normalize();
    }
    else
    {
        m_avoid*=0;
    }
}

void Boid::setGoal(ngl::Vec3 _goal)
{
    m_goal=_goal-m_position;
    if(m_goal!=0)
    {
        m_goal.normalize();
    }
}

void Boid::setFlee(ngl::Vec3 _flee)
{
    m_flee=m_position-_flee;
    if(m_flee!=0)
    {
        m_flee.normalize();
    }
}

void Boid::fleeWalls()
{
    if(m_position.m_x<=-85 || m_position.m_x>=85)
    {
        m_flee.m_x-=(m_position.m_x/2);
    }
    if(m_position.m_y<=-85 || m_position.m_y>=85)
    {
        m_flee.m_y-=(m_position.m_y/2);
    }
    if(m_position.m_z<=-85 || m_position.m_z>=85)
    {
        m_flee.m_z-=(m_position.m_z/2);
    }
}

void Boid::setEvade()
{

}

void Boid::setTarget()
{
    m_goal*=m_goalWeight;
    m_avoid*=MAX_AVOID_FORCE;
    m_separation*=m_separationWeight;
    m_align*=m_alignWeight;
    m_cohesion*=m_cohesionWeight;
    if(m_predator!=nullptr)
    {
        m_flee*=100/distance3d(m_predator->getPosition(), m_position);
    }
    m_target=m_separation+m_cohesion+m_align+m_avoid+m_goal+m_flee;
//    if(m_target.length()!=0)
//        m_target.normalize();
}

void Boid::setSteering()
{
    m_steering = m_target-m_velocity;
    if(m_steering.length()!=0)
    {
        m_steering.normalize();
    }
}

void Boid::updatePosition()
{
//    ngl::Vec2 last(m_position.m_x, m_position.m_z);
//    if(prevPos.size()<2)
//            prevPos.push_back(last);
//    else
//    {
//        prevPos[0]=prevPos[1];
//        prevPos[1]=last;
//    }
    m_velocity = m_velocity+(m_steering/m_mass);
    if(m_velocity.length()>MAX_SPEED)
    {
        m_velocity.normalize();
        m_velocity*=MAX_SPEED;
    }
    m_position = m_position+m_velocity;
}


void Boid::setRotate()
{
    m_yaw = atan2(m_velocity.m_x,m_velocity.m_z)*180/M_PI+180;
    m_pitch = atan2(m_velocity.m_y,sqrt(m_velocity.m_x*m_velocity.m_x+m_velocity.m_z*m_velocity.m_z))*180/M_PI;
    //ngl::Vec2 current(m_position.m_x, m_position.m_z);
    m_roll = 0;
//    if(prevPos.size()==2)
//    {
//        circlefrom3points(current, prevPos[0], prevPos[1]);
//        if(20>m_turnRadius>0.1)
//            m_roll = 90-(atan((m_mass*g)/m_turnRadius)*180/M_PI);
//    }
}


void Boid::move()
{
    m_flee.set(0,0,0);
    // perceptions
    if(hasLeader==true)
    {
        setGoal(m_leaderPos);
    }
    else if(m_neighbours.size()<1)
    {
        setGoal(m_flockCentroid);
    }
    if(m_predator!=nullptr && distance3d(m_predator->getPosition(), m_position)<55)
    {
        setFlee(m_predator->getPosition());
    }
    setSeparation();
    setAlign();
    setCohesion();
    setAvoid();
    fleeWalls();
    // drives
    setTarget();
    setSteering();
    // action selection
    updatePosition();
    setRotate();
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


float Boid::distance3d(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

float Boid::distance2d(ngl::Vec2 a, ngl::Vec2 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y)));
}

bool Boid::lineSphereIntersect(ngl::Vec3 ahead, ngl::Vec3 a, float radius)
{
    ngl::Vec3 ahead2=ahead*0.5;
    return distance3d(a, ahead) <= radius || distance3d(a, ahead2) <= radius || distance3d(a, m_position) <= radius;
}

void Boid::findObstacle(ngl::Vec3 ahead)
{
    m_collisionPos = NULL;
    for(int i=0;i<m_neighbours.size();++i)
    {

        ngl::Vec3 obstaclePos(m_neighbours[i]->getPosition());
        ngl::Vec3 temp = m_collisionPos;
        bool collision = lineSphereIntersect(ahead, obstaclePos, 10);
        if(collision==true  && (m_collisionPos==NULL || distance3d(m_position, obstaclePos) < distance3d(m_position, m_collisionPos)))
        {
            m_collisionPos=m_neighbours[i]->getPosition();
        }
        else if(m_collisionPos!=NULL)
            m_collisionPos=temp;
    }
}


void Boid::circlefrom3points(ngl::Vec2 A, ngl::Vec2 B, ngl::Vec2 C)
{
    // centre point (x, y) and radius r
    ngl::Vec2 centre;
    float r;

    //gradient of the line AB is mr and for BC it is mt
    float Mr = (B.m_y-A.m_y)/(B.m_x-A.m_x);
    float Mt = (C.m_y-B.m_y)/(C.m_x-B.m_x);

    // formula derived from equations for perpendicular
    // bisectors of chords AB and BC that intersect at the
    // centre
    centre.m_x = ((Mr*Mt*(C.m_y-A.m_y))+(Mr*(B.m_x+C.m_x))-(Mt*(A.m_x+B.m_x)))/(2*(Mr-Mt));
    // substitue x into equation for perpendicular bisector
    // of chord AB
    centre.m_y = (-1/Mr)*(centre.m_x-((A.m_x+B.m_x)/2))+((A.m_y+B.m_y)/2);
    // raidus is distance between and point A B C and the centre (x,y)
    m_turnRadius = distance2d(A,centre);
}







