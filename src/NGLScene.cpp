#include <QMouseEvent>
#include <QGuiApplication>
#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <flock.h>

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01f;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1f;

NGLScene::NGLScene(QWindow *_parent) : OpenGLWindow(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Flock");
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0.0f;
  m_spinYFace=0.0f;
}


NGLScene::~NGLScene()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  Init->NGLQuit();
}

void NGLScene::resizeEvent(QResizeEvent *_event )
{
  if(isExposed())
  {
      if(isExposed())
      {
      // set the viewport for openGL we need to take into account retina display
      // etc by using the pixel ratio as a multiplyer
      glViewport(0,0,width(),height());
      // now set the camera size values as the screen size has changed
      m_cam->setShape(45.0f,(float)width()/height(),0.05f,350.0f);
      renderLater();
      }
  }
}


void NGLScene::initialize()
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

  // initialise the flock
  m_flock = new Flock(10);


  // Camera position values
  ngl::Vec3 from(0,1,80);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  //load camera
  m_cam= new ngl::Camera(from,to,up);
  m_cam->setShape(45.0f, (float)720.0/576.0f,0.05f,350.0f);
  shader->setShaderParam3f("viewerPos", m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);
  // create a light
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  m_light = new ngl::Light(ngl::Vec3(15,20,70),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1), ngl::POINTLIGHT);
  m_light->setTransform(iv);
  //load light values to shader
  m_light->loadToShader("light");
  startTimer(10);
  // as re-size is not explicitly called we need to do this.
  glViewport(0,0,width(),height());


}

void NGLScene::loadMatricesToShader(int boidId)
{
    // create an instance of the shader lib
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 M;
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    M.scale(0.1,0.1,0.1);
    M+=m_mouseGlobalTX;
    M.translate(m_flock->m_flock[boidId].getXPos(), m_flock->m_flock[boidId].getYPos(), m_flock->m_flock[boidId].getZPos());
    MV = M*m_cam->getViewMatrix();
    MVP = M*m_cam->getVPMatrix();
    normalMatrix = MV;
    normalMatrix.inverse();
    //shader->setShaderParamFromMat4("M",M);
    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);

}


void NGLScene::render()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // create an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

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

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  //draw cube
  for(int i=0; i<m_flock->getSize(); ++i)
  {
    loadMatricesToShader(i);
    prim->draw("icosahedron");
  }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
        int diffx=_event->x()-m_origX;
        int diffy=_event->y()-m_origY;
        m_spinXFace+=(float)0.5f*diffy;
        m_spinYFace+=(float)0.5f*diffx;
        m_origX=_event->x();
        m_origY=_event->y();
        renderLater();
    }
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
        int diffX = (int)(_event->x() - m_origXPos);
        int diffY = (int)(_event->y() - m_origYPos);
        m_origXPos=_event->x();
        m_origYPos=_event->y();
        m_modelPos.m_x += INCREMENT * diffX;
        m_modelPos.m_y -= INCREMENT * diffY;
        renderLater();
    }

}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
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
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
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
void NGLScene::wheelEvent(QWheelEvent *_event)
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
    renderLater();

}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // space to update flock
  case Qt::Key_U : m_flock->updateFlock();
  // turn on wirframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  if (isExposed())
    renderLater();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::timerEvent(QTimerEvent *_e)
{
    m_flock->updateFlock();
    renderLater();
}
