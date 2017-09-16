#include <QDebug>
#include <propertyEditor.h>

Propertyeditor::Propertyeditor(Propertierbase* base, QWidget *parent): QDialog(parent)
{
  std::string helper;
  bool success;

  QFormLayout *data = new QFormLayout(this);

  const char **properties = base->names();
  puts("Properties: ")
  for(int i =0; i<base->property_count(); i++) {
    if(strcmp(base->type_name(properties[i]), "std::string") == 0) {
      std::string helper;
      bool success = false;
      std::string val = base->get(properties[i], &success, helper);

      if(success) {
	printf("%s = %s\n", properties[i], val.c_str());
      }
      else printf("%s failed\n", properties[i]);
    }
    else printf("typename %s is funny\n", base->type_name(properties[i]));
  }
  puts("Nomore properties");
  
  QHBoxLayout *l = new QHBoxLayout(this);
  QVBoxLayout *buttons = new QVBoxLayout(this);

  QPushButton *close = new QPushButton("Close", this);
  buttons->addWidget(close);
  l->addLayout(buttons);

  setLayout(l);
  setMinimumWidth(800);
  setMinimumHeight(600);

  connect(close, &QPushButton::clicked, this, &QDialog::accept);
}

Propertyeditor::~Propertyeditor()
{
  delete lbl;
}
