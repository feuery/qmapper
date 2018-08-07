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

class Propertyeditor: public QDialog{
  Q_OBJECT
public:

  Propertyeditor(cl_object base, QWidget *parent);
  ~Propertyeditor();

  virtual void showEvent(QShowEvent *e);

  std::function<void()> onClose = [&](){ accept(); };
  
private:

  std::unordered_map<std::string, QLabel*> field_errorlabel_mapping;

  QFormLayout* makeLayout(cl_object base);
  void resetLayout(cl_object base);

  QFormLayout *data;
  QVBoxLayout *l;

  void editingStdStringFinished(cl_object &base, std::string internedPropname, QLineEdit *edit);

};

#endif //PROPERTYEDITOR_H
