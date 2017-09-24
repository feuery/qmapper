#include <QDebug>
#include <QLabel>
#include <propertyEditor.h>
#include <QLineEdit>
#include <root.h>
#include <editorController.h>

#include <script-types.h>

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

either<bool, std::string> getScriptTypeAsString(Propertierbase *base, flyweight<std::string> internedPropname) {
  scriptTypes type = glsl;
  either<bool, std::string> result;
  switch(base->get(internedPropname, &result.a, type)) {
  case glsl:
    result.b = "glsl";
    break;
  case scheme:
    result.b = "scheme";
    break;
  }
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
    else if(type == "scriptTypes") {
      auto scriptTypeResult = getScriptTypeAsString(base, properties.at(i));
      QLabel *lbl = new QLabel((scriptTypeResult.a ? scriptTypeResult.b: "Error fetching value").c_str());
      data->addRow(QString(properties.at(i).get().c_str()), lbl);
    }
  }

  root *r = &editorController::instance->document;
  
  data->addRow(QString("Row: "),
	       new QLabel(QString(std::to_string(r->rowOf(base->getId())).c_str()), this));
  data->addRow(QString("Use row as parameter to the guile-api"), new QLabel(this));
  
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
