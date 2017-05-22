#ifndef MAIN_H
#define MAIN_H

#include <QDialog>
#include <renderer.h>
#include <guile_thread.h>

class MainWindow: public QDialog{
  Q_OBJECT
public:
  MainWindow(int argc, char** argv);
  ~MainWindow();

public slots:
  void setTexture_clicked(bool checked);
  
private:
  Renderer *r;
  Guile_Thread t;
  
};

#endif //MAIN_H
