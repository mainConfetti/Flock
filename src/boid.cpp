#include "boid.h"
#include <math.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

Boid::Boid()
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    m_AlignWeight=1;
    m_SeparationWeight=2;
    m_CohesionWeight=2.5;
    m_Speed=0.5;
    m_Mass=10;
    MAX_SEE_AHEAD = 2.5;
    MAX_AVOID_FORCE = 2;
    buildVAO();
}

Boid::Boid(int _id)
{
    setPos(0.0, 0.0, 0.0);
    m_Position.m_z = 0.0;
    setVelocity(1.0, 0.0, 0.0);
    setId(_id);
    m_AlignWeight=65;
    m_SeparationWeight=100;
    m_CohesionWeight=85;
    m_Speed=0.45;
    m_Mass=9;
    MAX_SEE_AHEAD =2;
    MAX_AVOID_FORCE = 100;
    buildVAO();
}

Boid::~Boid()
{

}


void Boid::setPos(float _x, float _y, float _z)
{
    m_Position.m_x = _x;
    m_Position.m_y = _y;
    m_Position.m_z = _z;
}

void Boid::setId(int _id)
{
    m_Id = _id;
}

int Boid::getId()
{
    return m_Id;
}

void Boid::setDistance(Boid *const boid)
{
    ngl::Vec3 boidPos = (boid->getPosition().m_x, boid->getPosition().m_y, boid->getPosition().m_z);
    m_Distance = Distance(m_Position, boidPos);
}

float Boid::getDistance()
{
    return m_Distance;
}

void Boid::setNeighbour(Boid *boid)
{
    m_Neighbours.push_back(boid);
}

void Boid::clearNeighbour()
{
    m_Neighbours.clear();
}

void Boid::getNeighbours()
{
    std::cout<< "boid: " << m_Id << " has neighbours: ";
    for(int i=0;i<m_Neighbours.size();++i)
        std::cout<<m_Neighbours[i]->getId()<<", ";
    std::cout<<std::endl;
}


void Boid::calcCentroid()
{
    m_Centroid = getPosition();
    if(m_Neighbours.size()>0)
    {
        for(int i=0;i<m_Neighbours.size();++i)
        {
            m_Centroid+=m_Neighbours[i]->getPosition();
        }
        m_Centroid /= m_Neighbours.size()+1;
    }
}

void Boid::calcCohesion()
{
    if(m_Neighbours.size()>0)
    {
        m_Cohesion = m_Centroid-m_Position;
        if(m_Cohesion!=0)
            m_Cohesion.normalize();
        m_Cohesion*=m_CohesionWeight;
    }
    else m_Cohesion.set(0,0,0);
}

void Boid::setVelocity(float _x, float _y, float _z)
{
    m_Velocity.m_x=_x;
    m_Velocity.m_y=_y;
    m_Velocity.m_z=_z;
}

void Boid::calcAlign()
{
    if(m_Neighbours.size()>0)
    {
        for(int i=0;i<m_Neighbours.size();++i)
        {
            m_Align+=m_Neighbours[i]->getVelocity();
        }
        m_Align /= m_Neighbours.size();
        if(m_Align!=0)
        m_Align.normalize();
        m_Align*=m_AlignWeight;
    }
    else
        m_Align.set(0,0,0);
}

void Boid::calcSeparation()
{
    if(m_Neighbours.size()>0)
    {
        for(int i=0;i<m_Neighbours.size();++i)
        {
        ngl::Vec3 pos(m_Neighbours[i]->getPosition());
        m_Neighbours[i]->setDistance(this);
        float weight = (1.0/m_Neighbours[i]->getDistance());
        ngl::Vec3 target = (pos-m_Position)*weight;
        m_Separation += target;
        }
        m_Separation /= m_Neighbours.size();
        if(m_Separation!=0)
        m_Separation.normalize();
        m_Separation = -m_Separation;
        m_Separation*=m_SeparationWeight;
    }
    else
        m_Separation.set(0,0,0);
}

void Boid::calcAvoid()
{
    ngl::Vec3 ahead;
    m_collisionPos = NULL;
    m_Velocity.normalize();
    ahead=m_Position + m_Velocity * MAX_SEE_AHEAD;
    findObstacle(ahead);
    m_avoid=(0,0,0);
    if(m_collisionPos != NULL)
    {
        m_avoid.m_x=(ahead.m_x-m_collisionPos.m_x); m_avoid.m_y=(ahead.m_y-m_collisionPos.m_y); m_avoid.m_z=(ahead.m_z-m_collisionPos.m_z);
        if(m_avoid.length()==0)
            m_avoid.m_x+=1;
        m_avoid.normalize();
    }
    else
    {
        m_avoid*=0;
    }
    if(ahead.m_x >= 50.0 || ahead.m_x <= -50.0)
    {
        m_avoid-=ahead*fabs(ahead.m_x/10);
    }
    if(ahead.m_y >= 50.0 || ahead.m_y <= -50.0)
    {
        m_avoid-=ahead*fabs(ahead.m_y/10);
    }
    if(ahead.m_z >= 50.0 || ahead.m_z <= -50.0)
    {
        m_avoid-=ahead*fabs(ahead.m_z/10);
    }
    m_avoid*=MAX_AVOID_FORCE;

}

void Boid::Info()
{
    std::cout<<"Pos: "<<m_Position.m_x<<", "<<m_Position.m_y<<", "<<m_Position.m_z<<std::endl;
    std::cout<<"Vel: "<<m_Velocity.m_x<<", "<<m_Velocity.m_y<<", "<<m_Velocity.m_z<<std::endl;
    std::cout<<"cetroid: "<<m_Centroid.m_x<<", "<<m_Centroid.m_y<<", "<<m_Centroid.m_z<<std::endl;
    std::cout<<"cohesion target: "<<m_Cohesion.m_x<<", "<<m_Cohesion.m_y<<", "<<m_Cohesion.m_z<<" : "<<m_Cohesion.length()<<std::endl;
    std::cout<<"align target: "<<m_Align.m_x<<", "<<m_Align.m_y<<", "<<m_Align.m_z<<" : "<<m_Align.length()<<std::endl;
    std::cout<<"separation target: "<<m_Separation.m_x<<", "<<m_Separation.m_y<<", "<<m_Separation.m_z<<" : "<<m_Separation.length()<<std::endl;
    std::cout<<"target vector: "<<m_Target.m_x<<", "<<m_Target.m_y<<", "<<m_Target.m_z<<std::endl;
    std::cout<<"steering vector: "<<m_Steering.m_x<<", "<<m_Steering.m_y<<", "<<m_Steering.m_z<<std::endl;
    std::cout<<"weights: sep - "<<m_SeparationWeight<<", coh - "<<m_CohesionWeight<<", align - "<<m_AlignWeight<<std::endl;
}

void Boid::setSWeight(int _separationWeight)
{
    m_SeparationWeight = _separationWeight;
}

void Boid::setCWeight(int _cohesionWeight)
{
    m_CohesionWeight = _cohesionWeight;
}

void Boid::setAWeight(int _alignWeight)
{
    m_AlignWeight = _alignWeight;
}

void Boid::setMass(int _mass)
{
    m_Mass = _mass;
}

int Boid::getSWeight()
{
   return m_SeparationWeight;
}

int Boid::getCWeight()
{
    return m_CohesionWeight;
}

int Boid::getAWeight()
{
    return m_AlignWeight;
}

int Boid::getMass()
{
    return m_Mass;
}

void Boid::setTarget()
{
    m_Target=m_Separation+m_Cohesion+m_Align+m_avoid;
    if(m_Target.length()!=0)
        m_Target.normalize();
}

void Boid::setSteering()
{
    m_Steering = m_Target-m_Velocity;
    if(m_Steering.length()!=0)
        m_Steering.normalize();
}

void Boid::updatePosition()
{
    m_Velocity = m_Velocity+(m_Steering/m_Mass);
    m_Velocity.normalize();
    m_Position = m_Position+(m_Velocity*m_Speed);
}

void Boid::setFlockCentroid(float _x, float _y, float _z)
 {
     m_FlockCentroid.m_x = _x;
     m_FlockCentroid.m_y = _y;
     m_FlockCentroid.m_z = _z;
     m_FlockCentroid.normalize();
 }

float Boid::Distance(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x)+(a.m_y - b.m_y)*(a.m_y - b.m_y) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

bool Boid::lineSphereIntersect(ngl::Vec3 ahead, ngl::Vec3 a, float radius)
{
    ngl::Vec3 ahead2=ahead*0.5;
    return Distance(a, ahead) <= radius || Distance(a, ahead2) <= radius || Distance(a, m_Position) <= radius;
}

void Boid::findObstacle(ngl::Vec3 ahead)
{
    m_collisionPos = NULL;
    int m;
    for(int i=0;i<m_Neighbours.size();++i)
    {

        ngl::Vec3 obstaclePos(m_Neighbours[i]->getPosition());
        bool collision = lineSphereIntersect(ahead, obstaclePos, 5);
        if(collision==1 && ((m_collisionPos=NULL)==true || Distance(m_Position, obstaclePos) < Distance(m_Position, m_collisionPos)))
        {
            m_collisionPos=m_Neighbours[i]->getPosition();
        }
    }
}

void Boid::buildVAO()
{
  ngl::Vec3 verts[]=
  {
    // face 1
    ngl::Vec3(0,0.5,-2),
    ngl::Vec3(-1,-0.5,2),
    ngl::Vec3(-1,0.5,2),
    // face 2
    ngl::Vec3(0,0.5,-1),
    ngl::Vec3(0,-0.5,-2),
    ngl::Vec3(-1,-0.5,2),
    // face 3
    ngl::Vec3(0,0.5,-2),
    ngl::Vec3(0,0.5,0.7),
    ngl::Vec3(-1,0.5,2),
    // face 4
    ngl::Vec3(0,0.5,-2),
    ngl::Vec3(0,0.5,0.7),
    ngl::Vec3(1,0.5,2),
    // face 5
    ngl::Vec3(0,-0.5,-2),
    ngl::Vec3(0,-0.5,0.7),
    ngl::Vec3(-1,-0.5,2),
    // face 6
    ngl::Vec3(0,-0.5,-2),
    ngl::Vec3(0,-0.5,0.7),
    ngl::Vec3(1,-0.5,2),
    // face 7
    ngl::Vec3(0,0.5,-2),
    ngl::Vec3(1,-0.5,2),
    ngl::Vec3(1,0.5,2),
    // face 8
    ngl::Vec3(0,0.5,-2),
    ngl::Vec3(0,-0.5,-2),
    ngl::Vec3(1,-0.5,2),
    // face 9
    ngl::Vec3(-1,0.5,2),
    ngl::Vec3(0,-0.5,0.7),
    ngl::Vec3(0,0.5,0.7),
    // face 10
    ngl::Vec3(-1,0.5,2),
    ngl::Vec3(-1,-0.5,2),
    ngl::Vec3(0,-0.5,0.7),
    // face 11
    ngl::Vec3(0,0.5,0.7),
    ngl::Vec3(1,-0.5,2),
    ngl::Vec3(1,0.5,2),
    // face 12
    ngl::Vec3(0,0.5,0.7),
    ngl::Vec3(0,-0.5,0.7),
    ngl::Vec3(1,-0.5,2)
  };

  std::vector <ngl::Vec3> normals;
  // face 1
  ngl::Vec3 n=ngl::calcNormal(verts[2],verts[1], verts[0]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 2
  n=ngl::calcNormal(verts[5],verts[4],verts[3]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 3
  n=ngl::calcNormal(verts[6],verts[7],verts[8]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 4
  n=ngl::calcNormal(verts[11],verts[10],verts[9]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 5
  n=ngl::calcNormal(verts[14],verts[13], verts[12]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 6
  n=ngl::calcNormal(verts[15],verts[16],verts[17]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 7
  n=ngl::calcNormal(verts[18],verts[19],verts[20]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 8
  n=ngl::calcNormal(verts[21],verts[22],verts[23]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 9
  n=ngl::calcNormal(verts[26],verts[25], verts[24]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 10
  n=ngl::calcNormal(verts[29],verts[28],verts[27]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 11
  n=ngl::calcNormal(verts[32],verts[31],verts[30]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
  // face 12
  n=ngl::calcNormal(verts[35],verts[34],verts[33]);
  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);

  std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
  // create a vao as a series of GL_TRIANGLES
  m_vao= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  m_vao->bind();

  // in this case we are going to set our data as the vertices above

    m_vao->setData(sizeof(verts),verts[0].m_x);
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

    m_vao->setData(sizeof(verts),normals[0].m_x);
    // now we set the attribute pointer to be 2 (as this matches normal in our shader)

    m_vao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);

    m_vao->setNumIndices(sizeof(verts)/sizeof(ngl::Vec3));

 // now unbind
  m_vao->unbind();


}

void Boid::draw()
{
    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();
}

void Boid::setRotate()
{
    m_rotate.reset();
    float rotY, rotX;
    rotY = atan(m_Velocity.m_z/m_Velocity.m_x)*180/M_PI;
    rotX = atan(m_Velocity.m_y/m_Velocity.m_z)*180/M_PI;
    m_rotate.setRotation(rotX, rotY, 0);
}























