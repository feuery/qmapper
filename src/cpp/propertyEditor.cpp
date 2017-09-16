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

either<bool, std::string> getStringProp(Propertierbase* base, const char *propname) {
  std::string lol;
  bool success = false;
  either<bool, std::string> result;
  result.b = base->get(propname, &success, lol);
  result.a = success;
  return result;
}

void editingStdStringFinished(Propertierbase *base, const char *propname, QLineEdit *edit) {
  base->set(propname, edit->text().toStdString());
}

Propertyeditor::Propertyeditor(Propertierbase* base, QWidget *parent): QDialog(parent)
{
  const char **properties = base->names();
  
  QVBoxLayout *l = new QVBoxLayout(this);
  QFormLayout *data = new QFormLayout(this);
  for(int i =0; i<base->property_count(); i++) {
    const char *type = base->type_name(properties[i]);
    if(strcmp(type, "std::string") == 0) {
      auto any = getStringProp(base, properties[i]);
      QLineEdit *edit = any.a ? new QLineEdit(QString(any.b.c_str()), this):
	new QLineEdit("Failed getting prop", this);

      connect(edit, &QLineEdit::editingFinished,
      	      [=]() { editingStdStringFinished(base, properties[i], edit); });
      
      data->addRow(properties[i], edit);
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
    activateWindow();
    setFocus(Qt::ActiveWindowFocusReason);
}
