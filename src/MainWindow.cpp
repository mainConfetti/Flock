#include "include/MainWindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>


MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  QGLFormat format;
  format.setVersion(4,1);
  format.setProfile( QGLFormat::CoreProfile);
  m_gl=new GLWindow(this);
  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);
  m_update=false;

  connect(m_ui->m_addBoid,SIGNAL(pressed()),this,SLOT(addBoidPressed()));
  connect(m_ui->m_removeBoid,SIGNAL(pressed()),m_gl,SLOT(removeBoid()));
  connect(m_ui->m_newFlock,SIGNAL(pressed()),this,SLOT(newFlockPressed()));
  connect(m_ui->m_leader,SIGNAL(toggled(bool)),m_gl,SLOT(toggleLeader(bool)));
  connect(m_ui->m_predator,SIGNAL(toggled(bool)),m_gl,SLOT(togglePredator(bool)));
  connect(m_ui->m_steer,SIGNAL(toggled(bool)),m_gl,SLOT(toggleSteer(bool)));
  connect(m_ui->m_cohesion,SIGNAL(sliderMoved(int)),m_gl,SLOT(setCohesion(int)));
  connect(m_ui->m_separation,SIGNAL(sliderMoved(int)),m_gl,SLOT(setseparation(int)));
  connect(m_ui->m_alignment,SIGNAL(sliderMoved(int)),m_gl,SLOT(setAlign(int)));
  connect(m_ui->m_speed,SIGNAL(sliderMoved(int)),m_gl,SLOT(setSpeed(int)));
  connect(m_ui->m_mass,SIGNAL(sliderMoved(int)),m_gl,SLOT(setMass(int)));
  connect(m_ui->m_sepDist,SIGNAL(sliderMoved(int)),m_gl,SLOT(setSepDist(int)));
  connect(m_ui->m_fov,SIGNAL(toggled(bool)),m_gl,SLOT(toggleFOV(bool)));
  connect(m_ui->m_fovAngle,SIGNAL(valueChanged(int)),m_gl,SLOT(setFOVAngle(int)));
  connect(m_ui->m_reset,SIGNAL(pressed()),this,SLOT(resetPressed()));

  connect(m_ui->m_addObstacle,SIGNAL(pressed()),m_gl,SLOT(addObstacle()));
  connect(m_ui->m_removeObstacle,SIGNAL(pressed()),m_gl,SLOT(removeObstacle()));
  connect(m_ui->m_clearObstacle,SIGNAL(pressed()),this,SLOT(clearObstaclePressed()));
  connect(m_ui->m_presetObstacles,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setObstacle(int)));
  connect(m_ui->m_addCustomObstacle,SIGNAL(pressed()),this,SLOT(addObstaclePressed()));
  connect(m_ui->m_removeCustomObstacle,SIGNAL(pressed()),this,SLOT(removeCustomObstaclePressed()));
  connect(m_ui->m_customStartX,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customStartY,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customStartZ,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customEndX,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customEndY,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customEndZ,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customCtrlX,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customCtrlY,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customCtrlZ,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customObsRad,SIGNAL(editingFinished()),this,SLOT(changeCustomObstaclePoints()));
  connect(m_ui->m_customObstacle,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObstacle(int)));

  connect(m_ui->m_showCtrlPoints,SIGNAL(toggled(bool)),m_gl,SLOT(toggleDrawPoints(bool)));

  connect(m_ui->m_visOctree,SIGNAL(toggled(bool)),m_gl,SLOT(toggleOctree(bool)));
  connect(m_ui->m_tails,SIGNAL(toggled(bool)),m_gl,SLOT(toggleTails(bool)));
  connect(m_ui->m_pov,SIGNAL(toggled(bool)),m_gl,SLOT(togglePOV(bool)));
  connect(m_ui->m_tailLength,SIGNAL(valueChanged(int)),m_gl,SLOT(setTrailLength(int)));
}

MainWindow::~MainWindow()
{
  delete m_ui;
}

void MainWindow::newFlockPressed()
{
  m_gl->newFlock(m_ui->m_flockCount->value(),
                 m_ui->m_cohesion->value(),
                 m_ui->m_separation->value(),
                 m_ui->m_alignment->value(),
                 (m_ui->m_speed->value()/10.0),
                 m_ui->m_mass->value());
}

void MainWindow::addBoidPressed()
{
  m_gl->addBoid(m_ui->m_cohesion->value(),
                m_ui->m_separation->value(),
                m_ui->m_alignment->value(),
                (m_ui->m_speed->value()/10.0),
                m_ui->m_mass->value());
}

void MainWindow::addObstaclePressed()
{
  if(m_ui->m_customObstacle->count()<=10)
  {
    m_gl->setNewCustomObstacle();
    QString item = QString::number((m_ui->m_customObstacle->count()));
    m_ui->m_customObstacle->addItem(item);
  }
}

void MainWindow::changeCustomObstaclePoints()
{
  if(m_update==true)
  {
    int id, rad;
    ngl::Vec3 start, end, ctrl;
    id = m_ui->m_customObstacle->currentIndex()-1;
    rad = m_ui->m_customObsRad->value();
    start.set(m_ui->m_customStartX->value(), m_ui->m_customStartY->value(), m_ui->m_customStartZ->value());
    end.set(m_ui->m_customEndX->value(), m_ui->m_customEndY->value(), m_ui->m_customEndZ->value());
    ctrl.set(m_ui->m_customCtrlX->value(), m_ui->m_customCtrlY->value(), m_ui->m_customCtrlZ->value());
    m_gl->updateObstacle(id, start, end, ctrl, rad);
  }
}

void MainWindow::selectObstacle(int _id)
{
  m_update=false;
  m_ui->m_customStartX->setValue(m_gl->m_customPoints[_id-1][0].m_x);
  m_ui->m_customStartY->setValue(m_gl->m_customPoints[_id-1][0].m_y);
  m_ui->m_customStartZ->setValue(m_gl->m_customPoints[_id-1][0].m_z);
  m_ui->m_customEndX->setValue(m_gl->m_customPoints[_id-1][1].m_x);
  m_ui->m_customEndY->setValue(m_gl->m_customPoints[_id-1][1].m_y);
  m_ui->m_customEndZ->setValue(m_gl->m_customPoints[_id-1][1].m_z);
  m_ui->m_customCtrlX->setValue(m_gl->m_customPoints[_id-1][2].m_x);
  m_ui->m_customCtrlY->setValue(m_gl->m_customPoints[_id-1][2].m_y);
  m_ui->m_customCtrlZ->setValue(m_gl->m_customPoints[_id-1][2].m_z);
  m_update=true;
}

void MainWindow::removeCustomObstaclePressed()
{
  int index = m_ui->m_customObstacle->count();
  std::cout<<index<<std::endl;
  if(index>1)
  {
  --index;
  m_ui->m_customObstacle->removeItem(index);
  }
  m_gl->removeCustomObstacle();
}

void MainWindow::clearObstaclePressed()
{
  int index = m_ui->m_customObstacle->count();
  for(int i=index-1; i>0; --i)
  {
    m_ui->m_customObstacle->removeItem(i);
  }
  m_gl->clearObstacle();
}

void MainWindow::resetPressed()
{
  m_ui->m_cohesion->setValue(200);
  m_ui->m_separation->setValue(100);
  m_ui->m_alignment->setValue(50);
  m_ui->m_sepDist->setValue(10);
  m_ui->m_speed->setValue(0.5);
  m_ui->m_mass->setValue(15);
  m_ui->m_fov->setCheckState(Qt::Unchecked);
  m_ui->m_fovAngle->setValue(100);
  m_gl->reset();
}
