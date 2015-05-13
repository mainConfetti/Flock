#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <GLWindow.h>
#include <ngl/Vec3.h>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *m_ui;
  GLWindow *m_gl;
  bool m_update;


private slots :
  void newFlockPressed();
  void addBoidPressed();
  void addObstaclePressed();
  void changeCustomObstaclePoints();
  void selectObstacle(int _id);

};

#endif // MAINWINDOW_H
