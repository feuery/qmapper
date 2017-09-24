#include <QDebug>
#include <QLabel>
#include <propertyEditor.h>
#include <QLineEdit>

template<typename T, typename E>
struct either
{
  T a;
  E b;
};

either<bool, std::string> getStringProp(Propertierbase* base, flyweight<std::string> internedPropname) {
  std::string lol;
  bool success = false;
  either<bool, std::string> result;
  result.b = base->get(internedPropname, &success, lol);
  result.a = success;
  return result;
}

void editingStdStringFinished(Propertierbase *base, flyweight<std::string> internedPropname, QLineEdit *edit) {

  base->set(internedPropname, edit->text().toStdString());
}

Propertyeditor::Propertyeditor(Propertierbase* base, QWidget *parent): QDialog(parent)
{
  std::vector<flyweight<std::string>> properties = base->names();
  
  QVBoxLayout *l = new QVBoxLayout(this);
  QFormLayout *data = new QFormLayout(this);
  for(int i =0; i<base->property_count(); i++) {
    flyweight<std::string> type = base->type_name(properties.at(i));
    if(type == std::string("std::string")) {
      auto any = getStringProp(base, properties.at(i));
      QLineEdit *edit = any.a ? new QLineEdit(QString(any.b.c_str()), this):
	new QLineEdit("Failed getting prop", this);

      connect(edit, &QLineEdit::editingFinished,
      	      [=]() { if(any.a) editingStdStringFinished(base, properties.at(i), edit); });
      
      data->addRow(QString(properties.at(i).get().c_str()), edit);
    }
  }
  
  QHBoxLayout *buttons = new QHBoxLayout(this);

  QPushButton *close = new QPushButton("Close", this);
  buttons->addWidget(close);

  l->addLayout(data);
  l->addLayout(buttons);
    
  setMinimumWidth(800);
  setMinimumHeight(600);

  connect(close, &QPushButton::clicked, this, &QDialog::accept);
}

Propertyeditor::~Propertyeditor()
{
}

void Propertyeditor::showEvent( QShowEvent* aShowEvent )
{
    QDialog::showEvent( aShowEvent );
    // activateWindow();
    // setFocus(Qt::ActiveWindowFocusReason);
}
