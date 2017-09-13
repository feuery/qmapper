#include <propertyEditor.h>

Propertyeditor::Propertyeditor(Propertierbase* base, QWidget *parent): QDialog(parent)
{
  std::string helper;
  bool success;
  std::string name = base->get("name", &success, &helper);
  
  lbl = new QLabel(this);
  if(success)
    lbl->setText(QString("Olio on %1").arg(name.c_str()));
  else
    lbl->setText(QString("Olio on %1").arg("virhe"));

  QHBoxLayout *l = new QHBoxLayout(this);
  QVBoxLayout *buttons = new QVBoxLayout(this);

  QPushButton *close = new QPushButton("Close", this);
  buttons->addWidget(close);
  l->addWidget(lbl);
  l->addLayout(buttons);

  setLayout(l);

  connect(close, &QPushButton::clicked, this, &QDialog::accept);
}

Propertyeditor::~Propertyeditor()
{
  delete lbl;
}
