#ifndef MAIN_H
#define MAIN_H

#include <QFrame>
#include <QGroupBox>
#include <renderer.h>
#include <guile_thread.h>
#include <editorController.h>

class editorController;

class MainWindow: public QFrame{
  Q_OBJECT
public:
  MainWindow(int argc, char** argv);
  ~MainWindow();

  void registerController(editorController *ec);
public slots:
  void setTexture_clicked(bool checked);
  
private:
  Renderer *r;
  Guile_Thread t;
  editorController *ec;

  QGroupBox* toolbox();
  
};

#endif //MAIN_H
