#include "GLWindow.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Light.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <QColorDialog>
#include <BoidMath.h>

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.5f;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=15.0f;
//----------------------------------------------------------------------------------------------------------------------

GLWindow::GLWindow(QWidget *_parent ) : QGLWidget( _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0.0f;
  m_spinYFace=0.0f;
  m_origX=0;
  m_origY=0;
  m_cameraIndex=0;
  m_currentObstacleId=-1;
  m_predator=false;
  m_leader=false;
  m_tails=false;
  m_vizOctree=false;
  m_width=this->size().width();
  m_height=this->size().height();
}

GLWindow::~GLWindow()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  delete m_world;
  delete m_Boidvao;
  delete m_tailVao;
  delete m_worldBounds;
  Init->NGLQuit();
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{

  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  // load shader and set values
  // create instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // create shader called Phong
  shader->createShaderProgram("Phong");
  // create empty frag and vert shaders
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach sources
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // addvshaders to program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // bind shader attributes for ngl primitives
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // link the shader
  shader->linkProgramObject("Phong");
  // use shader
  (*shader)["Phong"]->use();
  // load values (temp)
  shader->setShaderParam4f("objectColour", 1.0, 0.5, 0.31,1.0);

  //  (*shader)["nglColourShader"]->use();
  //  shader->setShaderParam4f("Colour", 1,1,1,1);
  m_world = new World(100);

  createCameras();

  shader->setRegisteredUniform("viewerPos", m_cameras[m_cameraIndex].getEye().toVec3());
  // create a light
  ngl::Mat4 iv=m_cameras[m_cameraIndex].getViewMatrix();
  iv.transpose();
  m_light = new ngl::Light(ngl::Vec3(800,1000,1000),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1), ngl::POINTLIGHT);
  m_light->setTransform(iv);
  //load light values to shader
  m_light->loadToShader("light");

  startTimer(10);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,m_width,m_height);

  m_worldBounds = ngl::VertexArrayObject::createVOA(GL_LINES);
  ngl::Vec3 verts[]=
  {
    ngl::Vec3(150,150,150),
    ngl::Vec3(150,150,-150),

    ngl::Vec3(150,150,-150),
    ngl::Vec3(-150,150,-150),

    ngl::Vec3(-150,150,-150),
    ngl::Vec3(-150,150,150),

    ngl::Vec3(-150,150,150),
    ngl::Vec3(150,150,150),

    ngl::Vec3(150,-150,150),
    ngl::Vec3(150,-150,-150),

    ngl::Vec3(150,-150,-150),
    ngl::Vec3(-150,-150,-150),

    ngl::Vec3(-150,-150,-150),
    ngl::Vec3(-150,-150,150),

    ngl::Vec3(-150,-150,150),
    ngl::Vec3(150,-150,150),

    ngl::Vec3(150,-150,150),
    ngl::Vec3(150,150,150),

    ngl::Vec3(150,-150,-150),
    ngl::Vec3(150,150,-150),

    ngl::Vec3(-150,-150,-150),
    ngl::Vec3(-150,150,-150),

    ngl::Vec3(-150,-150,150),
    ngl::Vec3(-150,150,150),
  };
  m_worldBounds->bind();
  m_worldBounds->setData(sizeof(verts),verts[0].m_x);
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

  m_worldBounds->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

  m_worldBounds->setNumIndices(sizeof(verts)/sizeof(ngl::Vec3));

  m_worldBounds->unbind();

  buildBoidVAO();

}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL( int _w, int _h )
{
  m_width = _w;
  m_height = _h;
  glViewport(0,0,_w,_h);
  m_cameras[0].setShape(45,float(_w/_h),0.05,350);
  m_cameras[1].setShape(80,float(_w/_h),0.01,350);
}


void GLWindow::loadMatricesToShader()
{
  // create an instance of the shader lib
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  //(*shader)["Phong"]->use();

  ngl::Mat4 M, MV, MVP;
  ngl::Mat3 normalMatrix;
  M=m_transform.getMatrix();
  if(m_cameraIndex==0)
  {
    MV = M*m_mouseGlobalTX*m_cameras[m_cameraIndex].getViewMatrix();
  }
  else
  {
    MV = M*m_cameras[m_cameraIndex].getViewMatrix();
  }
  MVP = MV*m_cameras[m_cameraIndex].getProjectionMatrix();
  normalMatrix = MV;
  normalMatrix.inverse();
  //shader->setShaderParamFromMat4("M",M);
  //shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);

}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // create an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  //(*shader)["Phong"]->use();

  //Rotation based on the mouse position
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  if(m_cameraIndex==1)
  {
    ngl::Vec3 boidPos = m_world->m_flock[2].getPosition();
    ngl::Vec3 povEye(boidPos.m_x, boidPos.m_y+2, boidPos.m_z);
    povEye-=m_world->m_flock[2].getVelocity()*3;
    ngl::Vec3 povLook= (m_world->m_flock[2].getVelocity()*40)+boidPos;
    ngl::Vec3 povUp(0.0f,1.0f,0.0f);
    m_cameras[m_cameraIndex].set(povEye,povLook,povUp);
  }
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("smallSphere",0.2,10);
  //draw flock
  for(int i=0; i<m_world->getSize(); ++i)
  {
    if(m_tails==true)
    {
      m_transform.reset();
      buildTailArray(i);
      for(int j=1;j<(m_tail.size());j+=1)
      {
        m_transform.setPosition(m_tail[j]);
        float remap = (float)j/(m_tail.size());
        m_transform.setScale(0.7*remap,0.7*remap,0.7*remap);
        loadMatricesToShader();
        shader->setShaderParam4f("objectColour",1.0,0.0,0.0,1.0);
        prim->draw("cube");
      }

    }
    if(m_world->m_flock[i].isLeader()==true)
    {
      m_transform.reset();
      m_transform.setScale(2,2,2);
      m_transform.setPosition(m_world->m_flock[i].getPosition());
      m_transform.addRotation( m_world->m_flock[i].getRotation());
      loadMatricesToShader();
      shader->setShaderParam4f("objectColour", 0.5, 0.5, 1,1.0);
      drawBoid();
    }
    else
    {
      m_transform.reset();
      m_transform.setPosition(m_world->m_flock[i].getPosition());
      m_transform.addRotation( m_world->m_flock[i].getRotation());
      loadMatricesToShader();
      shader->setShaderParam4f("objectColour", 1.0, 0.5, 0.31,1.0);
      drawBoid();
    }
  }

  //draw world
  m_transform.reset();
  loadMatricesToShader();
  shader->setShaderParam4f("objectColour", 0.0, 0.0, 0.0,1.0);
  m_worldBounds->bind();
  m_worldBounds->draw();
  m_worldBounds->unbind();

  //draw octree
  if(m_vizOctree==true)
  {
    m_transform.reset();
    loadMatricesToShader();
    m_world->drawOctree();
  }

  //draw predator
  if(m_predator==true)
  {
    m_transform.reset();
    m_transform.setScale(2,2,2);
    m_transform.setPosition(m_world->m_predator->getPosition());
    m_transform.setRotation(m_world->m_predator->getRotation());
    loadMatricesToShader();
    shader->setShaderParam4f("objectColour", 1.0, 0.1, 0.1,1.0);
    drawBoid();
  }

  //draw obstacles
  for(int i=0;i<m_world->m_obstacles.size();++i)
  {
    ngl::Vec3 colour = m_world->m_obstacles[i].getColour();
    shader->setShaderParam4f("objectColour", colour.m_x, colour.m_y, colour.m_z, 1.0);
    m_transform.reset();
    m_transform.setPosition(m_world->m_obstacles[i].getPosition());
    loadMatricesToShader();
    prim->draw("dodecahedron");
    float radius = m_world->m_obstacles[i].getRadius();
    m_transform.setScale(radius, radius, radius);
    loadMatricesToShader();
    prim->draw("dodecahedron");
  }
  for(int i=0;i<m_world->m_customObs.size();++i)
  {
    for(int j=0;j<m_world->m_customObs[i].size();++j)
    {
      ngl::Vec3 colour = m_world->m_customObs[i][j].getColour();
      shader->setShaderParam4f("objectColour", colour.m_x, colour.m_y, colour.m_z, 1.0);
      m_transform.reset();
      m_transform.setPosition(m_world->m_customObs[i][j].getPosition());
      loadMatricesToShader();
      prim->draw("dodecahedron");
      float radius = m_world->m_customObs[i][j].getRadius();
      m_transform.setScale(radius, radius, radius);
      loadMatricesToShader();
      prim->draw("dodecahedron");
    }
  }

  //draw custom obstacle control points
  if(m_drawPoints==true)
  {
    drawControlPoint(prim, shader);
  }

}





//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent ( QMouseEvent * _event )
{
  if(m_cameraIndex==0)
  {
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
      int diffx=_event->x()-m_origX;
      int diffy=_event->y()-m_origY;
      m_spinXFace+=(float)0.5f*diffy;
      m_spinYFace+=(float)0.5f*diffx;
      m_origX=_event->x();
      m_origY=_event->y();
      updateGL();
    }
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
      int diffX = (int)(_event->x() - m_origXPos);
      int diffY = (int)(_event->y() - m_origYPos);
      m_origXPos=_event->x();
      m_origYPos=_event->y();
      m_modelPos.m_x += INCREMENT * diffX;
      m_modelPos.m_y -= INCREMENT * diffY;
      updateGL();
    }
  }
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (QMouseEvent * _event  )
{

  // left button rotates
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate = true;
  }
  // right button translates
  if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate = true;
  }

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (  QMouseEvent * _event  )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{
  // check the diff of the wheel position (0 means no change)
  if(_event->delta() > 0)
  {
    m_modelPos.m_z+=ZOOM;
  }
  else if(_event->delta() <0 )
  {
    m_modelPos.m_z-=ZOOM;
  }
  updateGL();

}
//----------------------------------------------------------------------------------------------------------------------

void GLWindow::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
    // turn on wirframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
    // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
    // show full screen
  case Qt::Key_F : showFullScreen(); break;
    // show windowed
  case Qt::Key_N : showNormal(); break;
    // add predator
  case Qt::Key_P :
  {
    if(m_predator==false)
    {
      m_world->addPredator();
      m_predator=true;
    }
    else if(m_predator==true)
    {
      m_world->removePredator();
      m_predator=false;
    }
  } break;
  case Qt::Key_L :
  {
    if(m_leader==false)
    {
      m_leaderId = rand() % m_world->m_flock.size();
      m_world->setLeader(m_leaderId);
      m_leader=true;
    }
    else if(m_leader==true)
    {
      m_world->clearLeader();
      m_leader=false;
    }
  } break;
  case Qt::Key_O : m_world->addObstacle(); break;
  case Qt::Key_Return :
  {
    int index = (m_cameraIndex == 0 ? 1 : 0);
    m_cameraIndex = index;
  } break;
  case Qt::Key_Up : m_world->m_flock[m_leaderId].steerLeader(0); break;
  case Qt::Key_Down : m_world->m_flock[m_leaderId].steerLeader(1); break;
  case Qt::Key_Left : m_world->m_flock[m_leaderId].steerLeader(2); break;
  case Qt::Key_Right : m_world->m_flock[m_leaderId].steerLeader(3); break;
  case Qt::Key_Y : m_vizOctree = (m_vizOctree==true ? false : true); break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::timerEvent(QTimerEvent *_event)
{
  m_world->updateWorld();
  updateGL();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::buildBoidVAO()
{
  ngl::Vec3 verts[]=
  {
    // face 1
    ngl::Vec3(0,0.5,-2),
    ngl::Vec3(-1,-0.5,2),
    ngl::Vec3(-1,0.5,2),
    // face 2
    ngl::Vec3(0,0.5,-2),
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

  //std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
  // create a vao as a series of GL_TRIANGLES
  m_Boidvao= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  m_Boidvao->bind();

  // in this case we are going to set our data as the vertices above

  m_Boidvao->setData(sizeof(verts),verts[0].m_x);
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

  m_Boidvao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

  m_Boidvao->setData(sizeof(verts),normals[0].m_x);
  // now we set the attribute pointer to be 2 (as this matches normal in our shader)

  m_Boidvao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);

  m_Boidvao->setNumIndices(sizeof(verts)/sizeof(ngl::Vec3));

  // now unbind
  m_Boidvao->unbind();


}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::drawBoid()
{
  m_Boidvao->bind();
  m_Boidvao->draw();
  m_Boidvao->unbind();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::drawControlPoint(ngl::VAOPrimitives *_prim, ngl::ShaderLib *_shader)
{
  _prim->createSphere("sphere", 5, 20);
  _shader->setShaderParam4f("objectColour", 1.0,1.0,0.2,1.0);
  m_transform.reset();
  m_transform.setPosition(m_customPoints[m_currentObstacleId][2]);
  loadMatricesToShader();
  _prim->draw("sphere");
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::createCameras()
{
  // create a load of cameras
  ngl::Camera cam;
  ngl::Camera POVcam;
  // set the different vectors for the camera positions
  ngl::Vec3 eye(0.0f,1.0f,500.0f);
  ngl::Vec3 look=0.0f;
  ngl::Vec3 up(0,1,0);

  ngl::Vec3 povEye(0.0f,1.0f,500.0f);
  ngl::Vec3 povLook=0.0f;
  ngl::Vec3 povUp(0.0f,1.0f,0.0f);

  // finally set the cameras shape and position
  cam.set(eye,look,up);
  cam.setShape(45.0f, (float)m_width/m_height, 0.05f,350.0f);
  m_cameras.push_back(cam);
  POVcam.set(povEye,povLook,povUp);
  POVcam.setShape(80.0f, (float)m_width/m_height, 0.01f,350.0f);
  m_cameras.push_back(POVcam);
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::buildTailArray(int _id)
{
  m_tail.clear();
  std::deque<ngl::Vec3> tail(m_world->m_flock[_id].getPrevPos());
  for(int i=0;i<tail.size();++i)
  {
    m_tail.push_back(tail[i]);
  }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::addBoid(int _cohesion, int _separation,
                       int _alignment, float _speed,
                       int _mass)
{
  m_world->addBoid(_cohesion, _separation, _alignment, _speed, _mass);
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::removeBoid()
{
  m_world->removeBoid();
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::newFlock(int _numBoids, int _cohesion, int _separation,
                        int _alignment, float _speed, int _mass)
{
  delete m_world;
  m_world=new World(_numBoids, _cohesion, _separation,
                    _alignment, _speed, _mass);
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::toggleLeader(bool _leaderState)
{

  if(_leaderState==true)
  {
    m_leaderId = rand() % m_world->m_flock.size();
    m_world->setLeader(m_leaderId);
    m_leader=_leaderState;;
  }
  else
  {
    m_world->clearLeader();
    m_leader=_leaderState;;
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::togglePredator(bool _predatorState)
{
  if(_predatorState==true)
  {
    m_world->addPredator();
    m_predator=_predatorState;
  }
  else
  {
    m_world->removePredator();
    m_predator=_predatorState;
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::toggleSteer(bool _steerState)
{
  if(m_leader == true)
  {
    m_world->m_flock[m_leaderId].setSteer(_steerState);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::toggleFOV(bool _fovState)
{
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setFOV(_fovState);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::addObstacle()
{
  m_world->addObstacle();
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::removeObstacle()
{
  m_world->removeObstacle();
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::clearObstacle()
{
  m_world->clearObstacles();
  m_customPoints.clear();
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::removeCustomObstacle()
{
  m_world->removeCustomObstacle();
  if(m_customPoints.size()>0)
  {
    m_customPoints.pop_back();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::toggleOctree(bool _octreeState)
{
  m_vizOctree = _octreeState;
  setFocus();
}
void GLWindow::toggleTails(bool _tailState)
{
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setTail(_tailState);
  }
  m_tails = _tailState;
  setFocus();
}
void GLWindow::togglePOV(bool _povState)
{
  m_cameraIndex = _povState;
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::toggleDrawPoints(bool _drawPointsState)
{
  if(m_customPoints.size()!=0)
  {
    m_drawPoints = _drawPointsState;
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setFOVAngle(int _angle)
{
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setFovAngle(_angle);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setObstacle(int _index)
{
  m_world->createObstacles(_index);
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setCohesion(int _cohesion)
{
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setCWeight(_cohesion);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setseparation(int _separation)
{
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setSWeight(_separation);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setAlign(int _align)
{
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setAWeight(_align);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setSpeed(int _speed)
{
  float speed = _speed/10.0;
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setSpeed(speed);
  }
  if(m_predator==true)
  {
    m_world->m_predator->setSpeed(speed);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setMass(int _mass)
{
  for(int i=0; i<m_world->m_flock.size(); ++i)
  {
    m_world->m_flock[i].setMass(_mass);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setTrailLength(int _trailLength)
{
 for(int i=0;i<m_world->m_flock.size();++i)
 {
   m_world->m_flock[i].setTailLength(_trailLength);
 }
 setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setSepDist(int _sepDist)
{
  for(int i=0;i<m_world->m_flock.size();++i)
  {
    m_world->m_flock[i].setSepDist(_sepDist);
  }
  setFocus();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setNewCustomObstacle()
{
  if(m_customPoints.size()<10)
  {
    ngl::Vec3 start(0,-150,100);
    ngl::Vec3 end(0,-150,-100);
    ngl::Vec3 ctrl(0,100,0);
    std::vector<ngl::Vec3> customPoints;
    customPoints.push_back(start);
    customPoints.push_back(end);
    customPoints.push_back(ctrl);
    m_customPoints.push_back(customPoints);
    m_world->createCustomObstacle(customPoints[0],customPoints[1],customPoints[2], 10);
    m_currentObstacleId = (m_customPoints.size()-1);
  }
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateObstacle(int _id, ngl::Vec3 _newStart, ngl::Vec3 _newEnd, ngl::Vec3 _newCtrl, int _newRad)
{
  m_world->updateCustomObstacle(_id, _newStart, _newEnd, _newCtrl, _newRad);
  m_customPoints[_id][0]=_newStart;
  m_customPoints[_id][1]=_newEnd;
  m_customPoints[_id][2]=_newCtrl;
  setFocus();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::reset()
{
  if(m_predator==true)
  {
    m_world->removePredator();
  }
  if(m_leader==true)
  {
    m_world->clearLeader();
  }
  for(int i=0;i<m_world->m_flock.size();++i)
  {
    m_world->m_flock[i].setCWeight(200);
    m_world->m_flock[i].setSWeight(100);
    m_world->m_flock[i].setAWeight(50);
    m_world->m_flock[i].setSpeed(0.5);
    m_world->m_flock[i].setMass(15);
    m_world->m_flock[i].setSepDist(10);
    m_world->m_flock[i].setFOV(0);
    m_world->m_flock[i].setFovAngle(100);
  }
}













