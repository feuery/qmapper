#include <QDebug>
#include <QLabel>
#include <propertyEditor.h>
#include <limits>
#include <QComboBox>
#include <QCheckBox>
#include <editorController.h>

#include <tilelistmodel.h>
#include <either.h>
#include <QDoubleValidator>
#include <QStandardItemModel>

std::string getStringProp(SCM base, std::string internedPropname)
{
  return scm_to_locale_string(get(base, internedPropname.c_str()));
}

bool getBoolProp(SCM base, std::string internedPropname)
{ 
  return scm_is_true(get(base, internedPropname.c_str())) == 1;
}

int getIntProp(SCM base, std::string internedPropname)
{
  return scm_to_int(get(base, internedPropname.c_str()));
}

double getDoubleProp(SCM base, std::string internedPropname)
{
  return scm_to_double(get(base, internedPropname.c_str()));
}

std::string getScriptTypeAsString(SCM base, std::string internedPropname)
{
  static SCM scriptType = scm_c_lookup("Script-script_type");
  const char *toret = scm_to_locale_string(scm_symbol_to_string(get(base, internedPropname.c_str())));
  return std::string(toret);
}
  

void Propertyeditor::editingStdStringFinished(SCM &base, std::string internedPropname, QLineEdit *edit)
{
  base = set(base, internedPropname.c_str(), scm_from_locale_string(edit->text().toStdString().c_str()));
  puts("TODO error handling is not done");
  // std::string errors = base->getErrorsOf(internedPropname);
  // if(QLabel *l = field_errorlabel_mapping[internedPropname]) {
  //   l->setText(QString(errors.c_str()));
  // }
  // else
  //   printf("No error-label found for %s\n", internedPropname.c_str());
  // base->clearErrorsOf(internedPropname);
}

void editingIntNmbrStringFinished(SCM &base, std::string propName, QLineEdit *l) {
  QString t = l->text();

  bool canConvert = false;
  int i = t.toInt(&canConvert);

  if(!canConvert) {
    qDebug() << "Can't convert property " << propName.c_str() << "with value " << t << " to int";
    return;
  }

  int val = (int)i;
  qDebug() << "Setting " << propName.c_str() << " to " << val;
  base = set(base, propName.c_str(), scm_from_int(val));
}

void editingNmbrStringFinished(SCM &base, std::string propName, QLineEdit *l) {
  QString t = l->text();

  bool canConvert = false;
  int i = t.toInt(&canConvert);

  if(!canConvert) {
    qDebug() << "Can't convert property " << propName.c_str() << "with value " << t << " to int";
    return;
  }

  double val = (double)i;
  qDebug() << "Setting " << propName.c_str() << " to " << val;
  base = set(base, propName.c_str(), scm_from_double(val));
}

void editingBoolFinished(SCM &base, std::string propname, bool checked) {
  base = set(base, propname.c_str(), scm_from_bool(checked));
}

QStandardItemModel* dump_to_model_vertical()
{
  QStandardItemModel *model = new QStandardItemModel;

  QStandardItem *empty = new QStandardItem("Empty");
  model->appendRow(empty);

  QStandardItem *m = new QStandardItem("Top");
  QVariant var;
  var.setValue(QString("TOP"));
  m->setData(var);
  model->appendRow(m);

  m = new QStandardItem("Bottom");
  QVariant var2;
  var2.setValue(QString("BOTTOM"));
  m->setData(var2);
  model->appendRow(m);

  return model;
}

QStandardItemModel* dump_to_model_horizontal()
{
  QStandardItemModel *model = new QStandardItemModel;

  QStandardItem *empty = new QStandardItem("Empty");
  model->appendRow(empty);

  QStandardItem *m = new QStandardItem("Left");
  QVariant var;
  var.setValue(QString("LEFT"));
  m->setData(var);
  model->appendRow(m);

  m = new QStandardItem("Right");
  QVariant var2;
  var2.setValue(QString("RIGHT"));
  m->setData(var2);
  model->appendRow(m);

  return model;
}

// QStandardItemModel* dump_to_model(std::vector<SCM>* prop_objs)
// {
//   if(!prop_objs) {
//     qDebug() << "prop_objs is null at " << __FILE__ << ": " << __LINE__;
//     throw "";
//   }
//   QStandardItemModel *model = new QStandardItemModel;

//   QStandardItem *empty = new QStandardItem("Empty");
//   model->appendRow(empty);

//   for(auto m = prop_objs->begin(); m < prop_objs->end(); m++) {
//     either<bool, std::string> result = getStringProp(*m, std::string(std::string("name")));
    
//     QStandardItem *map_item = new QStandardItem(result.b.c_str());
//     QVariant var;
//     var.setValue(*m);
//     map_item->setData(var);
//     model->appendRow(map_item);
//   }

//   return model;
// }

std::vector<std::string> keys(SCM b)
{
    static SCM keys = scm_c_lookup("keys-str");
    static SCM list_ref = scm_c_lookup("list-ref");
    static SCM len = scm_c_lookup("length");
    
    SCM k_res = scm_call_1(keys, b);
    int length = scm_to_int(scm_call_1(len, k_res));

    std::vector<std::string> result;
    for(int i = 0; i < length; i++) {
      std::string str(scm_to_locale_string(scm_call_2(list_ref,
						      k_res,
						      scm_from_int(i))));
      result.push_back(str);
    }
    return result;
}

static void indexChanged(SCM &b, std::string internedPropName, SCM editedObject)
{
  b = set(b, internedPropName.c_str(), editedObject);

  qDebug()<<"Successfully changed " << internedPropName.c_str();
}

QFormLayout* Propertyeditor::makeLayout(SCM base) {

  QFormLayout *data = new QFormLayout;

  std::vector<std::string> properties = keys(base);

  static SCM str = scm_c_lookup("prop-str?");
  static SCM list = scm_c_lookup("prop-list?");
  static SCM prop_number = scm_c_lookup("prop-number?");
  static SCM prop_bool = scm_c_lookup("prop-bool?");
  static SCM prop_float = scm_c_lookup("prop-float?");
  static SCM prop_sym = scm_c_lookup("prop-sym?");

  for(int i =0; i<properties.size(); i++) {

    if(scm_is_true(scm_call_2(list, base, scm_from_locale_symbol(properties.at(i).c_str()))) == 1)
      continue;
    
    if(scm_is_true(scm_call_2(str,
			      base,
			      scm_from_locale_symbol(properties.at(i).c_str()))) == 1) {
      auto any = getStringProp(base, properties.at(i));
      QLineEdit *edit = new QLineEdit(QString(any.c_str()), this);

      connect(edit, &QLineEdit::editingFinished,
      	      [&]() { editingStdStringFinished(base, properties.at(i), edit); });

      std::string fieldName = properties.at(i);
      // std::string errors = base->getErrorsOf(fieldName);

      // QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      // error_field->setStyleSheet("QLabel { color: red; }");

      // field_errorlabel_mapping[properties.at(i)] = error_field;
      
      data->addRow(QString(properties.at(i).c_str()), edit);
      // data->addRow(QString(""), error_field);
    }
    else if (scm_is_true(scm_call_2(prop_bool, base, scm_from_locale_symbol(properties.at(i).c_str()))) == 1) {
      bool b = getBoolProp(base, properties.at(i));

      QCheckBox *cb = new QCheckBox(properties.at(i).c_str(), this);
      cb->setCheckState(b ? Qt::Checked: Qt::Unchecked);

      connect(cb, &QCheckBox::stateChanged,
	      [&](int state) { editingBoolFinished(base, properties.at(i), state == Qt::Checked); });

      data->addRow("", cb);
      
    }
    // This probably could be macrofied for all the numeric types
    else if (scm_is_true(scm_call_2(prop_number, base, scm_from_locale_symbol(properties.at(i).c_str()))) == 1) {
      int v = getIntProp(base, properties.at(i));
      
      QLineEdit *edit = new QLineEdit(QString::number(v), this);

      // edit->setValidator(new QIntValidator(0, 255, this));

      connect(edit, &QLineEdit::editingFinished,
	      [&]() { editingNmbrStringFinished(base, properties.at(i), edit); });

      std::string fieldName = properties.at(i);
      // std::string errors = base->getErrorsOf(fieldName);

      // QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      // error_field->setStyleSheet("QLabel { color: red; }");

      // field_errorlabel_mapping[properties.at(i)] = error_field;
      
      data->addRow(QString(properties.at(i).c_str()), edit);
      // data->addRow(QString(""), error_field);
    }
    else if (scm_is_true(scm_call_2(prop_float, base, scm_from_locale_symbol(properties.at(i).c_str()))) == 1) {
      double v = getDoubleProp(base, properties.at(i));
      
      QLineEdit *edit = new QLineEdit(QString::number(v), this);

      // edit->setValidator(new QDoubleValidator(std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), 15, this));

      connect(edit, &QLineEdit::editingFinished,
      	      [&]() { 
		  editingNmbrStringFinished(base, properties.at(i), edit);
		});

      std::string fieldName = properties.at(i);
      // std::string errors = base->getErrorsOf(fieldName);

      // QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      // error_field->setStyleSheet("QLabel { color: red; }");

      // field_errorlabel_mapping[properties.at(i)] = error_field;
      
      data->addRow(QString(properties.at(i).c_str()), edit);
      // data->addRow(QString(""), error_field);
    }
    else if(scm_is_true(scm_call_2(prop_sym, base, scm_from_locale_symbol(properties.at(i).c_str()))) == 1) {
      std::string scriptTypeResult = getScriptTypeAsString(base, properties.at(i));
      QLabel *lbl = new QLabel(scriptTypeResult.c_str());
      data->addRow(QString(properties.at(i).c_str()), lbl);
    }
    else if(scm_is_true(scm_call_2(str, base, scm_from_locale_symbol(properties.at(i).c_str()))) == 1) {
      std::string scriptTypeResult = getStringProp(base, properties.at(i));
      QLabel *lbl = new QLabel(scriptTypeResult.c_str());
      data->addRow(QString(properties.at(i).c_str()), lbl);
    }
    // else if(type == "verticalAnchor") {
    //   verticalAnchor lol = TOP;
    //   bool succ = false;
    //   verticalAnchor currentVal = base->get(properties.at(i), &succ, lol);
    //   std::string propName = properties.at(i);

    //   QComboBox *cb = new QComboBox(this);
    //   QStandardItemModel *m = dump_to_model(TOP);
    //   cb->setModel(m);

    //   connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    // 	      [=](int index) {
    // 		index--;
    // 		if(index >= 0) {
    // 		  verticalAnchor ha = (verticalAnchor)index;
		  
    // 		  base->set(propName, ha);
    // 		}
    // 		else qDebug() << "index < 0";
    // 	      });

    //   QVariant v;
    //   v.setValue(currentVal);
    //   int index = cb->findData(v);
    //   if(index > -1) cb->setCurrentIndex(index);
    //   else qDebug() << "No valid index found for verticalAnchor";

    //   data->addRow(QString(properties.at(i).c_str()), cb);
    // }
    // else if(type == "horizontalAnchor") {
    //   horizontalAnchor lol = LEFT;
    //   bool succ = false;
    //   horizontalAnchor currentVal = base->get(properties.at(i), &succ, lol);
    //   std::string propName = properties.at(i);

    //   QComboBox *cb = new QComboBox(this);
    //   QStandardItemModel *m = dump_to_model(lol);
    //   cb->setModel(m);

    //   connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    // 	      [=](int index) {
    // 		index--;
    // 		if(index >= 0) {
    // 		  horizontalAnchor ha = (horizontalAnchor)index;
		  
    // 		  base->set(propName, ha);
    // 		}
    // 		else qDebug() << "index < 0";
    // 	      });

    //   QVariant v;
    //   v.setValue(currentVal);
    //   int index = cb->findData(v);
    //   if(index > -1) cb->setCurrentIndex(index);
    //   else qDebug() << "No valid index found for horizontalAnchor";

    //   data->addRow(QString(properties.at(i).c_str()), cb);
    // }
    else {
      puts("löl");
      // SCM r = &editorController::instance->document;

      // SCM currentValue = get(base, properties.at(i).c_str());
      // std::string nameResult;
      // if (currentValue) {
      // 	nameResult = getStringProp(currentValue, "name");
      // }
      // else {
      // 	nameResult = "Empty";
      // }
      
      // std::vector<SCM>* kids = r->registryOf(type);
      // if(!kids) {
      // 	qDebug() << "Kids of " << type.c_str() << " is null at " << __FILE__ << ": " << __LINE__;
      // 	throw "";
      // }

      // QComboBox *cb = new QComboBox(this);
      // QStandardItemModel *m = dump_to_model(kids);
      // cb->setModel(m);
      
      // int index = cb->findText(nameResult.b.c_str());
      // if(index > -1) cb->setCurrentIndex(index);
      // else qDebug() << "No valid index found for " << nameResult.b.c_str();
      
      // connect(cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      // 	      [&](int index) {
      // 		index--;
      // 		if(index >= 0) {
      // 		  Propertierbase *b = kids->at(index);
      // 		  if(!b) {
      // 		    qDebug() << "invalid b in QComboBox::currentIndexChanged";
      // 		    return;
      // 		  }

      // 		  indexChanged(b, properties.at(i), base);
      // 		}
      // 		else indexChanged(nullptr, properties.at(i), base);
      // 	      });
	      
      // data->addRow(QString(properties.at(i).c_str()), cb);
    }
  }

  return data;
}

void Propertyeditor::resetLayout(SCM base) {
  SCM r = editorController::instance->document;
  SCM regToList = scm_c_lookup("root-registryToList");
  auto reg = scm_call_1(regToList, r);
  l = new QVBoxLayout;
  
  data = makeLayout(base);

  // data->addRow(QString("Row: "),
  // 	       new QLabel(QString(std::to_string(indexOf<SCM>(&reg, base)).c_str()), this));
  // data->addRow(QString("Use row as parameter to the guile-api"), new QLabel(this));
  
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

Propertyeditor::Propertyeditor(SCM base, QWidget *parent): QDialog(parent)
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
