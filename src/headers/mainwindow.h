#ifndef MAIN_H
#define MAIN_H

#include <QDialog>
#include <renderer.h>

class MainWindow: public QDialog{
  Q_OBJECT
public:
  MainWindow();

public slots:
  void setTexture_clicked(bool checked);
  
private:
  Renderer *r;
  
};

#endif //MAIN_H
