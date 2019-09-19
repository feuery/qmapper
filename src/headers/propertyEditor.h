#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QDialog>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <string>
#include <QLineEdit>
#include <unordered_map>
#include <guile_fn.h>
#include <ui_propeditorui.h>

class Propertyeditor: public QDialog{
  Q_OBJECT
public:

  Propertyeditor(cl_object list_path_to_object, QWidget *parent);
  ~Propertyeditor();

  virtual void showEvent(QShowEvent *e);

  std::function<void()> onClose = [&](){ accept(); };
  
private:

  Ui::propeditor ui;

  // std::unordered_map<std::string, QLabel*> field_errorlabel_mapping;

};

#endif //PROPERTYEDITOR_H
