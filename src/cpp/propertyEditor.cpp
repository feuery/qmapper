#include <QDebug>
#include <QLabel>
#include <propertyEditor.h>
#include <QComboBox>
#include <QCheckBox>
#include <root.h>
#include <editorController.h>

#include <script-types.h>
#include <tilelistmodel.h>
#include <either.h>
#include <script.h>
#include <tileset.h>
#include <tilesetContainer.h>

either<bool, std::string> getStringProp(Propertierbase* base, flyweight<std::string> internedPropname) {
  if(internedPropname.get() == std::string("Id")) return {true, base->getId().get()};
  
  std::string lol;
  bool success = false;
  either<bool, std::string> result;
  result.b = base->get(internedPropname, &success, lol);
  result.a = success;
  return result;
}

either<bool, bool> getBoolProp(Propertierbase* base, flyweight<std::string> internedPropname) { 
  bool success = false;
  either<bool, bool> result;
  result.b = base->get(internedPropname, &success, success);
  result.a = success;
  return result;
}

template<typename T>
either<bool, T> getNumericProp(Propertierbase* base, flyweight<std::string> internedPropname) {
  T t;
  bool success = false;
  either<bool, T> result;
  result.b = base->get(internedPropname, &success, t);
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
  

void Propertyeditor::editingStdStringFinished(Propertierbase *base, flyweight<std::string> internedPropname, QLineEdit *edit)
{
  base->set(internedPropname, edit->text().toStdString());
  std::string errors = base->getErrorsOf(internedPropname);
  if(QLabel *l = field_errorlabel_mapping[internedPropname.get()]) {
    l->setText(QString(errors.c_str()));
  }
  else
    printf("No error-label found for %s\n", internedPropname.get().c_str());
  base->clearErrorsOf(internedPropname);
}

template <typename T>
void editingNmbrStringFinished(Propertierbase *base, flyweight<std::string> propName, QLineEdit *l) {
  QString t = l->text();

  bool canConvert = false;
  int i = t.toInt(&canConvert);

  if(!canConvert) {
    qDebug() << "Can't convert property " << propName.get().c_str() << "with value " << t << " to int";
    return;
  }

  T val = (T)i;
  base->set(propName, val);
}

void editingBoolFinished(Propertierbase *base, flyweight<std::string> propname, bool checked) {
  base->set(propname, checked);
}

QStandardItemModel* dump_to_model(std::vector<Propertierbase*> prop_objs)
{
  QStandardItemModel *model = new QStandardItemModel;

  QStandardItem *empty = new QStandardItem("Empty");
  model->appendRow(empty);

  for(auto m = prop_objs.begin(); m < prop_objs.end(); m++) {
    either<bool, std::string> result = getStringProp(*m, flyweight<std::string>(std::string("name")));
    
    QStandardItem *map_item = new QStandardItem(result.b.c_str());
    QVariant var;
    var.setValue(*m);
    map_item->setData(var);
    model->appendRow(map_item);
  }

  return model;
}

static void indexChanged(Propertierbase *b, flyweight<std::string> internedPropName, Propertierbase *editedObject)
{
  editedObject->set(internedPropName, b);

  // if(b->type_identifier().get() == std::string("Script") &&
  //    editedObject->type_identifier().get() == std::string("Tileset") &&
  //    toScript(b)->getScript_type() == glsl) {
  //   tilesetContainer *t = static_cast<tilesetContainer*>(editedObject);
  //   auto f = editorController::instance->getGlFns();
  //   static_cast<obj*>(t)->reload_shaders(f, t->getVertexshader()->getId(), t->getFragmentshader()->getId());
  // }
  
  qDebug()<<"Successfully changed " << internedPropName.get().c_str();
}

QFormLayout* Propertyeditor::makeLayout(Propertierbase *base) {
  QFormLayout *data = new QFormLayout;

  std::vector<flyweight<std::string>> properties = base->names();

  for(int i =0; i<base->property_count(); i++) {
    std::string type = base->type_name(properties.at(i)).get();
    
    if(type == std::string("std::string")) {
      auto any = getStringProp(base, properties.at(i));
      QLineEdit *edit = any.a ? new QLineEdit(QString(any.b.c_str()), this):
	new QLineEdit("Failed getting prop", this);

      connect(edit, &QLineEdit::editingFinished,
      	      [=]() { if(any.a) editingStdStringFinished(base, properties.at(i), edit); });

      flyweight<std::string> fieldName = properties.at(i);
      std::string errors = base->getErrorsOf(fieldName);

      QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      error_field->setStyleSheet("QLabel { color: red; }");

      field_errorlabel_mapping[properties.at(i).get()] = error_field;
      
      data->addRow(QString(properties.at(i).get().c_str()), edit);
      data->addRow(QString(""), error_field);
    }
    else if (type == "bool") {
      auto result = getBoolProp(base, properties.at(i));

      if(!result.a) {
	qDebug() << "Couldn't get bool prop " << properties.at(i).get().c_str();
	throw "";
      }

      bool b = result.b;
      QCheckBox *cb = new QCheckBox(properties.at(i).get().c_str(), this);

      connect(cb, &QCheckBox::stateChanged,
	      [=](int state) { if(result.a) editingBoolFinished(base, properties.at(i), state == Qt::Checked); });

      data->addRow("", cb);
      
    }
    // This probably could be macrofied for all the numeric types
    else if (type == "unsigned char") {
      auto result = getNumericProp<unsigned char>(base, properties.at(i));

      if(!result.a) {
	qDebug() << "Couldn't find unsigned char - prop " << properties.at(i).get().c_str();
	throw "";
      }

      int v = result.b;
      
      QLineEdit *edit = result.a? new QLineEdit(QString::number(v), this):
	new QLineEdit("Failed getting prop", this);

      edit->setValidator(new QIntValidator(0, 255, this));

      connect(edit, &QLineEdit::editingFinished,
	      [=]() { if(result.a) editingNmbrStringFinished<unsigned char>(base, properties.at(i), edit); });

      flyweight<std::string> fieldName = properties.at(i);
      std::string errors = base->getErrorsOf(fieldName);

      QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      error_field->setStyleSheet("QLabel { color: red; }");

      field_errorlabel_mapping[properties.at(i).get()] = error_field;
      
      data->addRow(QString(properties.at(i).get().c_str()), edit);
      data->addRow(QString(""), error_field);
    }      
    else if(type == "scriptTypes") {
      auto scriptTypeResult = getScriptTypeAsString(base, properties.at(i));
      QLabel *lbl = new QLabel((scriptTypeResult.a ? scriptTypeResult.b: "Error fetching value").c_str());
      data->addRow(QString(properties.at(i).get().c_str()), lbl);
    }
    else if(type == "flyweight<std::string>") {
      auto scriptTypeResult = getStringProp(base, properties.at(i));
      QLabel *lbl = new QLabel((scriptTypeResult.a ? scriptTypeResult.b: "Error fetching value").c_str());
      data->addRow(QString(properties.at(i).get().c_str()), lbl);
    }
    else {
      root *r = &editorController::instance->document;

      Propertierbase *currentValue = base->get(properties.at(i));
      either<bool, std::string> nameResult;
      if (currentValue) {
	nameResult = getStringProp(currentValue, flyweight<std::string>(std::string("name")));
      }
      else {
	nameResult = {true, "Empty"};
      }
      if(!nameResult.a) continue;
      
      std::vector<Propertierbase*> kids = r->registryOf(type);

      QComboBox *cb = new QComboBox(this);
      QStandardItemModel *m = dump_to_model(kids);
      cb->setModel(m);
      
      int index = cb->findText(nameResult.b.c_str());
      if(index > -1) cb->setCurrentIndex(index);
      else qDebug() << "No valid index found for " << nameResult.b.c_str();
      
      connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	      [&](int index) {
		index--;
		if(index >= 0) {
		  Propertierbase *b = kids.at(index);
		  if(!b) {
		    qDebug() << "invalid b in QComboBox::currentIndexChanged";
		    return;
		  }

		  indexChanged(b, properties.at(i), base);
		}
		else indexChanged(nullptr, properties.at(i), base);
	      });
	      
      data->addRow(QString(properties.at(i).get().c_str()), cb);
    }
  }

  return data;
}

void Propertyeditor::resetLayout(Propertierbase *base) {
  root *r = &editorController::instance->document;
  auto reg = r->registryToList({});
  l = new QVBoxLayout;
  
  data = makeLayout(base);

  data->addRow(QString("Row: "),
	       new QLabel(QString(std::to_string(indexOf<Propertierbase*>(&reg, base)).c_str()), this));
  data->addRow(QString("Use row as parameter to the guile-api"), new QLabel(this));
  
  QHBoxLayout *buttons = new QHBoxLayout;

  QPushButton *close = new QPushButton("Close", this);
  buttons->addWidget(close);
  
  l->addLayout(data);
  l->addLayout(buttons);
  
  setLayout(l);

  connect(close, &QPushButton::clicked, this, &QDialog::accept);
}

Propertyeditor::Propertyeditor(Propertierbase* base, QWidget *parent): QDialog(parent)
{  
  resetLayout(base);
    
  setMinimumWidth(800);
  setMinimumHeight(600);
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
