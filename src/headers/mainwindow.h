#ifndef MAIN_H
#define MAIN_H

#include <QtWidgets/QDialog>
#include "ui_main.h"

class MainWindow: public QMainWindow{
  Q_OBJECT
public:
  MainWindow();
  
private:
  Ui::MainWindow ui;
};

#endif //MAIN_H
