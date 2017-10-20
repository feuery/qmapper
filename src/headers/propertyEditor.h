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
#include <QLineEdit>
#include <unordered_map>

class Propertyeditor: public QDialog{
  Q_OBJECT
public:

  Propertyeditor(Propertierbase* base, QWidget *parent);
  ~Propertyeditor();

  virtual void showEvent(QShowEvent *e);
  
private:

  std::unordered_map<std::string, QLabel*> field_errorlabel_mapping;

  QFormLayout* makeLayout(Propertierbase *base);
  void resetLayout(Propertierbase *base);

  QFormLayout *data;
  QVBoxLayout *l;

  void editingStdStringFinished(Propertierbase *base, flyweight<std::string> internedPropname, QLineEdit *edit);

};

#endif //PROPERTYEDITOR_H
