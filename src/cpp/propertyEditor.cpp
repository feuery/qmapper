#include <QDebug>
#include <propertyEditor.h>

Propertyeditor::Propertyeditor(Propertierbase* base, QWidget *parent): QDialog(parent)
{
  std::string helper;
  bool success;

  QFormLayout *data = new QFormLayout(this);

  // const char[][] properties = base->names();

  // QDebug()<<"Propertyjä on " << (sizeof(properties)/sizeof(properties[0]) << " jäsentä";
  
  QHBoxLayout *l = new QHBoxLayout(this);
  QVBoxLayout *buttons = new QVBoxLayout(this);

  QPushButton *close = new QPushButton("Close", this);
  buttons->addWidget(close);
  l->addLayout(buttons);

  setLayout(l);

  connect(close, &QPushButton::clicked, this, &QDialog::accept);
}

Propertyeditor::~Propertyeditor()
{
  delete lbl;
}
