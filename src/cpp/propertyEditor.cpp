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

either<bool, std::string> getStringProp(Propertierbase* base, std::string internedPropname) {
  if(internedPropname == "Id") return {true, base->getId()};
  
  std::string lol;
  bool success = false;
  either<bool, std::string> result;
  result.b = base->get(internedPropname, &success, lol);
  result.a = success;
  return result;
}

either<bool, bool> getBoolProp(Propertierbase* base, std::string internedPropname) { 
  bool success = false;
  either<bool, bool> result;
  result.b = base->get(internedPropname, &success, success);
  result.a = success;
  return result;
}

template<typename T>
either<bool, T> getNumericProp(Propertierbase* base, std::string internedPropname) {
  T t;
  bool success = false;
  either<bool, T> result;
  result.b = base->get(internedPropname, &success, t);
  result.a = success;
  return result;
}

either<bool, std::string> getScriptTypeAsString(Propertierbase *base, std::string internedPropname) {
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
  

void Propertyeditor::editingStdStringFinished(Propertierbase *base, std::string internedPropname, QLineEdit *edit)
{
  base->set(internedPropname, edit->text().toStdString());
  std::string errors = base->getErrorsOf(internedPropname);
  if(QLabel *l = field_errorlabel_mapping[internedPropname]) {
    l->setText(QString(errors.c_str()));
  }
  else
    printf("No error-label found for %s\n", internedPropname.c_str());
  base->clearErrorsOf(internedPropname);
}

template <typename T>
void editingNmbrStringFinished(Propertierbase *base, std::string propName, QLineEdit *l) {
  QString t = l->text();

  bool canConvert = false;
  int i = t.toInt(&canConvert);

  if(!canConvert) {
    qDebug() << "Can't convert property " << propName.c_str() << "with value " << t << " to int";
    return;
  }

  T val = (T)i;
  qDebug() << "Setting " << propName.c_str() << " to " << val;
  base->set(propName, val);
}

void editingBoolFinished(Propertierbase *base, std::string propname, bool checked) {
  base->set(propname, checked);
}

QStandardItemModel* dump_to_model(verticalAnchor v_a) {
  QStandardItemModel *model = new QStandardItemModel;

  QStandardItem *empty = new QStandardItem("Empty");
  model->appendRow(empty);

  QStandardItem *m = new QStandardItem("Top");
  QVariant var;
  var.setValue(TOP);
  m->setData(var);
  model->appendRow(m);

  m = new QStandardItem("Bottom");
  QVariant var2;
  var2.setValue(BOTTOM);
  m->setData(var2);
  model->appendRow(m);

  return model;
}

QStandardItemModel* dump_to_model(horizontalAnchor v_a) {
  QStandardItemModel *model = new QStandardItemModel;

  QStandardItem *empty = new QStandardItem("Empty");
  model->appendRow(empty);

  QStandardItem *m = new QStandardItem("Left");
  QVariant var;
  var.setValue(LEFT);
  m->setData(var);
  model->appendRow(m);

  m = new QStandardItem("Right");
  QVariant var2;
  var2.setValue(RIGHT);
  m->setData(var2);
  model->appendRow(m);

  return model;
}

QStandardItemModel* dump_to_model(std::vector<Propertierbase*> prop_objs)
{
  QStandardItemModel *model = new QStandardItemModel;

  QStandardItem *empty = new QStandardItem("Empty");
  model->appendRow(empty);

  for(auto m = prop_objs.begin(); m < prop_objs.end(); m++) {
    either<bool, std::string> result = getStringProp(*m, std::string(std::string("name")));
    
    QStandardItem *map_item = new QStandardItem(result.b.c_str());
    QVariant var;
    var.setValue(*m);
    map_item->setData(var);
    model->appendRow(map_item);
  }

  return model;
}

static void indexChanged(Propertierbase *b, std::string internedPropName, Propertierbase *editedObject)
{
  editedObject->set(internedPropName, b);

  // if(b->type_identifier() == std::string("Script") &&
  //    editedObject->type_identifier() == std::string("Tileset") &&
  //    toScript(b)->getScript_type() == glsl) {
  //   tilesetContainer *t = static_cast<tilesetContainer*>(editedObject);
  //   auto f = editorController::instance->getGlFns();
  //   static_cast<obj*>(t)->reload_shaders(f, t->getVertexshader()->getId(), t->getFragmentshader()->getId());
  // }
  
  qDebug()<<"Successfully changed " << internedPropName.c_str();
}

QFormLayout* Propertyeditor::makeLayout(Propertierbase *base) {
  QFormLayout *data = new QFormLayout;

  std::vector<std::string> properties = base->names();

  for(int i =0; i<base->property_count(); i++) {
    std::string type = base->type_name(properties.at(i));
    
    if(type == std::string("std::string")) {
      auto any = getStringProp(base, properties.at(i));
      QLineEdit *edit = any.a ? new QLineEdit(QString(any.b.c_str()), this):
	new QLineEdit("Failed getting prop", this);

      connect(edit, &QLineEdit::editingFinished,
      	      [=]() { if(any.a) editingStdStringFinished(base, properties.at(i), edit); });

      std::string fieldName = properties.at(i);
      std::string errors = base->getErrorsOf(fieldName);

      QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      error_field->setStyleSheet("QLabel { color: red; }");

      field_errorlabel_mapping[properties.at(i)] = error_field;
      
      data->addRow(QString(properties.at(i).c_str()), edit);
      data->addRow(QString(""), error_field);
    }
    else if (type == "bool") {
      auto result = getBoolProp(base, properties.at(i));

      if(!result.a) {
	qDebug() << "Couldn't get bool prop " << properties.at(i).c_str();
	throw "";
      }

      bool b = result.b;
      QCheckBox *cb = new QCheckBox(properties.at(i).c_str(), this);
      cb->setCheckState(b ? Qt::Checked: Qt::Unchecked);

      connect(cb, &QCheckBox::stateChanged,
	      [=](int state) { if(result.a) editingBoolFinished(base, properties.at(i), state == Qt::Checked); });

      data->addRow("", cb);
      
    }
    // This probably could be macrofied for all the numeric types
    else if (type == "unsigned char") {
      auto result = getNumericProp<unsigned char>(base, properties.at(i));

      if(!result.a) {
	qDebug() << "Couldn't find unsigned char - prop " << properties.at(i).c_str();
	throw "";
      }

      int v = result.b;
      
      QLineEdit *edit = result.a? new QLineEdit(QString::number(v), this):
	new QLineEdit("Failed getting prop", this);

      edit->setValidator(new QIntValidator(0, 255, this));

      connect(edit, &QLineEdit::editingFinished,
	      [=]() { if(result.a) editingNmbrStringFinished<unsigned char>(base, properties.at(i), edit); });

      std::string fieldName = properties.at(i);
      std::string errors = base->getErrorsOf(fieldName);

      QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      error_field->setStyleSheet("QLabel { color: red; }");

      field_errorlabel_mapping[properties.at(i)] = error_field;
      
      data->addRow(QString(properties.at(i).c_str()), edit);
      data->addRow(QString(""), error_field);
    }
    else if (type == "int") {
      auto result = getNumericProp<int>(base, properties.at(i));

      if(!result.a) {
	qDebug() << "Couldn't find int - prop " << properties.at(i).c_str();
	throw "";
      }

      int v = result.b;
      
      QLineEdit *edit = result.a? new QLineEdit(QString::number(v), this):
	new QLineEdit("Failed getting prop", this);

      edit->setValidator(new QIntValidator(0, INT_MAX, this));

      connect(edit, &QLineEdit::editingFinished,
      	      [=]() { if(result.a) {
		  qDebug() << "Basen tyyppi: " << base->type_identifier().c_str();
		  editingNmbrStringFinished<int>(base, properties.at(i), edit);
		}});

      std::string fieldName = properties.at(i);
      std::string errors = base->getErrorsOf(fieldName);

      QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      error_field->setStyleSheet("QLabel { color: red; }");

      field_errorlabel_mapping[properties.at(i)] = error_field;
      
      data->addRow(QString(properties.at(i).c_str()), edit);
      data->addRow(QString(""), error_field);
    }
    else if(type == "scriptTypes") {
      auto scriptTypeResult = getScriptTypeAsString(base, properties.at(i));
      QLabel *lbl = new QLabel((scriptTypeResult.a ? scriptTypeResult.b: "Error fetching value").c_str());
      data->addRow(QString(properties.at(i).c_str()), lbl);
    }
    else if(type == "std::string") {
      auto scriptTypeResult = getStringProp(base, properties.at(i));
      QLabel *lbl = new QLabel((scriptTypeResult.a ? scriptTypeResult.b: "Error fetching value").c_str());
      data->addRow(QString(properties.at(i).c_str()), lbl);
    }
    else if(type == "verticalAnchor") {
      verticalAnchor lol = TOP;
      bool succ = false;
      verticalAnchor currentVal = base->get(properties.at(i), &succ, lol);
      std::string propName = properties.at(i);

      QComboBox *cb = new QComboBox(this);
      QStandardItemModel *m = dump_to_model(TOP);
      cb->setModel(m);

      connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	      [=](int index) {
		index--;
		if(index >= 0) {
		  verticalAnchor ha = (verticalAnchor)index;
		  
		  base->set(propName, ha);
		}
		else qDebug() << "index < 0";
	      });

      QVariant v;
      v.setValue(currentVal);
      int index = cb->findData(v);
      if(index > -1) cb->setCurrentIndex(index);
      else qDebug() << "No valid index found for verticalAnchor";

      data->addRow(QString(properties.at(i).c_str()), cb);
    }
    else if(type == "horizontalAnchor") {
      horizontalAnchor lol = LEFT;
      bool succ = false;
      horizontalAnchor currentVal = base->get(properties.at(i), &succ, lol);
      std::string propName = properties.at(i);

      QComboBox *cb = new QComboBox(this);
      QStandardItemModel *m = dump_to_model(lol);
      cb->setModel(m);

      connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	      [=](int index) {
		index--;
		if(index >= 0) {
		  horizontalAnchor ha = (horizontalAnchor)index;
		  
		  base->set(propName, ha);
		}
		else qDebug() << "index < 0";
	      });

      QVariant v;
      v.setValue(currentVal);
      int index = cb->findData(v);
      if(index > -1) cb->setCurrentIndex(index);
      else qDebug() << "No valid index found for horizontalAnchor";

      data->addRow(QString(properties.at(i).c_str()), cb);
    }
    else {
      root *r = &editorController::instance->document;

      Propertierbase *currentValue = base->get(properties.at(i));
      either<bool, std::string> nameResult;
      if (currentValue) {
	nameResult = getStringProp(currentValue, "name");
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
	      
      data->addRow(QString(properties.at(i).c_str()), cb);
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

  connect(close, &QPushButton::clicked, [&]() {
      onClose();
    });
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
