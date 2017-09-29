#include <QDebug>
#include <QLabel>
#include <propertyEditor.h>
#include <QLineEdit>
#include <QComboBox>
#include <root.h>
#include <editorController.h>

#include <script-types.h>
#include <tilelistmodel.h>

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

QStandardItemModel* dump_to_model(std::vector<Propertierbase*>* prop_objs)
{
  QStandardItemModel *model = new QStandardItemModel;

  QStandardItem *empty = new QStandardItem("Empty");
  model->appendRow(empty);

  for(auto m = prop_objs->begin(); m < prop_objs->end(); m++) {
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
  qDebug()<<"Successfully changed " << internedPropName.get().c_str();
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
      
      std::vector<Propertierbase*>* kids = new std::vector<Propertierbase*>;

      for(auto it = r->registry->begin(); it != r->registry->end(); it++) {
	if(it->second->type_identifier() == type) {
	  kids->push_back(it->second);
	} 
      };      

      QComboBox *cb = new QComboBox(this);
      QStandardItemModel *m = dump_to_model(kids);
      cb->setModel(m);
      
      int index = cb->findText(nameResult.b.c_str());
      if(index > -1) cb->setCurrentIndex(index);
      else qDebug() << "No valid index found for " << nameResult.b.c_str();
      
      connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	      [=](int index) {
		index--;
		if(index >= 0) {
		  Propertierbase *b = kids->at(index);
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
