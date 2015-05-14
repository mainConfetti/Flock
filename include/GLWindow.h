/// @file GLWindow.h
/// @brief a basic Qt GL window class for ngl demos
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class GLWindow
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file

#ifndef GL_WINDOW_H__
#define GL_WINDOW_H__
#include <ngl/Camera.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QEvent>
#include <QResizeEvent>
#include <QGLWidget>
#include <ngl/Text.h>
#include <QtOpenGL>
#include <World.h>


class GLWindow : public QGLWidget
{
  Q_OBJECT        // must include this if you use Qt signals/slots
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for GLWindow
  /// @param [in] _parent the parent window to create the GL context in
  //----------------------------------------------------------------------------------------------------------------------
  GLWindow(QWidget *_parent );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~GLWindow();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This method is called to create a new flock
  /// @param[in] _numBoids the numder of boinds in the new flock
  /// @param[in] _cohesion the cohesion weight to set to each boid
  /// @param[in] _separation the separation weight to set to each boid
  /// @param[in] _alignment the alignment weight to set to each boid
  /// @param[in] _speed the speed value to set to each boid
  /// @param[in] _mass the mass value to set to each boid
  void newFlock(int _numBoids, int _cohesion, int _separation, int _alignment, float _speed, int _mass);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A method to add a boid
  /// @param[in] _cohesion the cohesion weight to set
  /// @param[in] _separation the separation weight to set
  /// @param[in] _alignment the alignment weight to set
  /// @param[in] _speed the speed value to set
  /// @param[in] _mass the mass value to set
  void addBoid(int _cohesion, int _separation, int _alignment, float _speed, int _mass);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A method to set a new custom obstacle from a bezier curve
  /// @return sets default values for the bezier curve start, end and control
  /// vertices to m_customPoints
  void setNewCustomObstacle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A method update a custom obstacle form the user imput
  /// @return updates the obstacle positions and radii by calling updateCustomObstacle from World class
  /// also updates the relevent ngl::vec3 values in m_customPoints
  void updateObstacle(int _id, ngl::Vec3 _newStart, ngl::Vec3 _newEnd, ngl::Vec3 _newCtrl, int _newRad);

  std::vector<std::vector<ngl::Vec3>> m_customPoints;

public slots :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to remove a boid
  void removeBoid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle Leader
  /// @param[in] _leaderState the leader state (leader or no leader)
  /// passed from the toggle button
  void toggleLeader(bool _leaderState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle Predator
  /// @param[in] _predatorState the predator state (predator or no predator)
  /// passed from the toggle button
  void togglePredator(bool _predatorState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle steerable leader
  /// @param[in] _steerState the steer state (on or off)
  /// passed from the toggle button
  void toggleSteer(bool _steerState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle boid field of view
  /// @param[in] _fovState the field of view state (on or off)
  /// passed from the toggle button
  void toggleFOV(bool _fovState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle octree visualisation
  /// @param[in] _octreeState the visualisation state (on or off)
  /// passed from the toggle button
  void toggleOctree(bool _octreeState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle tails
  /// @param[in] _tailState the tail state (on or off)
  /// passed from the toggle button
  void toggleTails(bool _tailState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle point of view camera
  /// @param[in] _povState the point of view state (on or off)
  /// passed from the toggle button
  void togglePOV(bool _povState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to toggle drawing of control vertices of custom obstacles
  /// @param[in] _drawPointsState the draw points state (on or off)
  /// passed from the toggle button
  void toggleDrawPoints(bool _drawPointsState);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to add an obstacle
  void addObstacle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to remove an obstacle
  void removeObstacle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to clear all obstacles
  void clearObstacle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to set boid field of view angle
  /// @param[in] _angle the value to set
  void setFOVAngle(int _angle);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to create preset obstacle courses
  /// @param[in] _index the index of the preset course
  void setObstacle(int _index);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to set cohesion weight
  /// @param[in] _cohesion the value to set
  void setCohesion(int _cohesion);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to set separation
  /// @param[in] _separation the value to set
  void setseparation(int _separation);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to set alignment
  /// @param[in] _align the value to set
  void setAlign(int _align);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to set speed
  /// @param[in] _speed the value to set
  void setSpeed(int _speed);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A slot to set mass
  /// @param[in] _mass the value to set
  void setMass(int _mass);


protected :

  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL();

  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  void resizeGL(const int _w, const int _h );
  /// @brief this is the main gl drawing routine which is called whenever the window needs to
  // be re-drawn
  void paintGL();

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our world containing the flock
  //----------------------------------------------------------------------------------------------------------------------
  World *m_world;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a container for our Cameras
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <ngl::Camera > m_cameras;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief an index into the active camera list
  //----------------------------------------------------------------------------------------------------------------------
  int m_cameraIndex;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the x rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinXFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the y rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinYFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_rotate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the Right mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_translate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origY;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origXPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origYPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the global mouse transforms
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_mouseGlobalTX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the model position for mouse movement
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The light
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Light *m_light;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our transform for objects
  ngl::Transformation m_transform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief load required matrices to the shader program
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is pressed
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent (QMouseEvent * _event );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent ( QMouseEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent ( QMouseEvent *_event );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent( QWheelEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime there is a qttimer Event.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void timerEvent(QTimerEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Vertex Array Object for the boid
  //----------------------------------------------------------------------------------------------------------------------
  ngl::VertexArrayObject *m_Boidvao;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Vertex Array Object for the boid's tails
  //----------------------------------------------------------------------------------------------------------------------
  ngl::VertexArrayObject *m_tailVao;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Vertex Array Object for the world bounds
  //----------------------------------------------------------------------------------------------------------------------
  ngl::VertexArrayObject *m_worldBounds;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to build the boid vao
  //----------------------------------------------------------------------------------------------------------------------
  void buildBoidVAO();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to build the array that contains the previous positions of a boid
  /// in order to draw a tail
  /// @param [in] _id the id of the boid whos tail is being created
  //----------------------------------------------------------------------------------------------------------------------
  void buildTailArray(int _id);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to draw the boid VAO
  //----------------------------------------------------------------------------------------------------------------------
  void drawBoid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to signal if there is a predator in the system
  //----------------------------------------------------------------------------------------------------------------------
  bool m_predator;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to signal if the flock has a leader
  //----------------------------------------------------------------------------------------------------------------------
  bool m_leader;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to signal whether to draw the boid tails
  //----------------------------------------------------------------------------------------------------------------------
  bool m_tails;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to signal whether to draw the octree
  //----------------------------------------------------------------------------------------------------------------------
  bool m_vizOctree;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to signal whether to draw custom object control points
  //----------------------------------------------------------------------------------------------------------------------
  bool m_drawPoints;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector containing previous positions of a boid to draw its trail
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> m_tail;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to create and set up the cameras for the scene
  //----------------------------------------------------------------------------------------------------------------------
  void createCameras();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the boid ID of the flock's leader
  //----------------------------------------------------------------------------------------------------------------------
  int m_leaderId;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the height in pixels of the window
  //----------------------------------------------------------------------------------------------------------------------
  int m_height;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the width in pixels of the window
  //----------------------------------------------------------------------------------------------------------------------
  int m_width;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the radius of the custom obstacle currently being processed
  //----------------------------------------------------------------------------------------------------------------------
  int m_customObstacleRad;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the id of the custom obstacle currently being processed
  //----------------------------------------------------------------------------------------------------------------------
  int m_currentObstacleId;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to draw the custom obstacle control points
  //----------------------------------------------------------------------------------------------------------------------
  void drawControlPoint(ngl::VAOPrimitives *_prim,ngl::ShaderLib *_shader);


};

#endif
