#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QDialog>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <propertierbase.h>
#include <string>

class Propertyeditor: public QDialog{
  Q_OBJECT
public:

  Propertyeditor(Propertierbase* base, QWidget *parent);
  ~Propertyeditor();

  virtual void showEvent(QShowEvent *e);
  
private:

};

#endif //PROPERTYEDITOR_H
