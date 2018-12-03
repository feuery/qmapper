#include <QDebug>
#include <QLabel>
#include <propertyEditor.h>
#include <limits>
#include <QComboBox>
#include <QCheckBox>
#include <editorController.h>

#include <either.h>
#include <QDoubleValidator>
#include <QStandardItemModel>

std::string getStringProp(cl_object base, std::string internedPropname)
{
  return ecl_string_to_string(get(base, internedPropname.c_str()));
}

bool getBoolProp(cl_object base, std::string internedPropname)
{ 
  return get(base, internedPropname.c_str()) == ECL_T;
}

int getIntProp(cl_object base, std::string internedPropname)
{
  return ecl_to_int(get(base, internedPropname.c_str()));
}

double getDoubleProp(cl_object base, std::string internedPropname)
{
  return ecl_to_double(get(base, internedPropname.c_str()));
}

std::string getScriptTypeAsString(cl_object base, std::string internedPropname)
{
  static cl_object scriptType = makefn("qmapper.script:Script-script_type");
  return  ecl_string_to_string(cl_symbol_name(get(base, internedPropname.c_str())));
}
  

void Propertyeditor::editingStdStringFinished(cl_object base, std::string internedPropname, QLineEdit *edit)
{
  std::string value = "\""+edit->text().toStdString()+"\"";
  // printf("setting obj's %s to %s\n", internedPropname.c_str(), value.c_str());
  base = set(base, internedPropname.c_str(), c_string_to_object(value.c_str()));
  // set toimii lopultakin oikein
  // nyt vaan pitäis dumpata tää cl_objecti takaisin documenttiin



  
  // auto format = makefn("format");
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"edited object is ~a ~%\""), base);

  
  // std::string errors = base->getErrorsOf(internedPropname);
  // if(QLabel *l = field_errorlabel_mapping[internedPropname]) {
  //   l->setText(QString(errors.c_str()));
  // }
  // else
  //   printf("No error-label found for %s\n", internedPropname.c_str());
  // base->clearErrorsOf(internedPropname);
}

void editingIntNmbrStringFinished(cl_object &base, std::string propName, QLineEdit *l) {
  QString t = l->text();

  bool canConvert = false;
  int i = t.toInt(&canConvert);

  if(!canConvert) {
    qDebug() << "Can't convert property " << propName.c_str() << "with value " << t << " to int";
    return;
  }

  int val = (int)i;
  qDebug() << "Setting " << propName.c_str() << " to " << val;
  base = set(base, propName.c_str(), ecl_make_int(val));
}

void editingNmbrStringFinished(cl_object &base, std::string propName, QLineEdit *l) {
  QString t = l->text();

  bool canConvert = false;
  int i = t.toInt(&canConvert);

  if(!canConvert) {
    qDebug() << "Can't convert property " << propName.c_str() << "with value " << t << " to int";
    return;
  }

  double val = (double)i;
  qDebug() << "Setting " << propName.c_str() << " to " << val;
  base = set(base, propName.c_str(), ecl_make_double_float(val));
}

void editingBoolFinished(cl_object &base, std::string propname, bool checked) {
  base = set(base, propname.c_str(), ecl_make_bool(checked));
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

// QStandardItemModel* dump_to_model(std::vector<cl_object>* prop_objs)
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

std::vector<std::string> keys(cl_object b)
{
  cl_object keys = makefn("qmapper.std:keys-str");
  cl_object car = makefn("car"),
    cdr = makefn("cdr");
  cl_object len = makefn("common-lisp:length");
    
  cl_object k_res = cl_funcall(2, keys, b);
  int length = fixint(cl_funcall(2, len, k_res));

  std::vector<std::string> result;
  cl_object format = makefn("common-lisp:format");
  cl_funcall(4, format, ECL_T, c_string_to_object("\"key-collection is ~a~%\""), k_res);
  for(int i = 0; i < length; i++) {
    auto str = ecl_string_to_string(cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""), cl_funcall(2, car, k_res)));
    k_res = cl_funcall(2, cdr, k_res);
    printf("found key %s\n", str.c_str());
    result.push_back(str);
  }
  return result;
}

static void indexChanged(cl_object &b, std::string internedPropName, cl_object editedObject)
{
  b = set(b, internedPropName.c_str(), editedObject);

  qDebug()<<"Successfully changed " << internedPropName.c_str();
}

QFormLayout* Propertyeditor::makeLayout(cl_object base) {

  QFormLayout *data = new QFormLayout;
  cl_object format = makefn("common-lisp:format");

  // cl_funcall(4, format, ECL_T, c_string_to_object("\"building layout for ~a~%\""), base);

  std::vector<std::string> properties = keys(base);

  cl_object str = makefn("qmapper.std:prop-str?"),
    list = makefn("qmapper.std:prop-list?"),
    prop_number = makefn("qmapper.std:prop-number?"),
    prop_bool = makefn("qmapper.std:prop-bool?"),
    prop_float = makefn("qmapper.std:prop-float?"),
    prop_sym = makefn("qmapper.std:prop-sym?");

  for(int i =0; i<properties.size(); i++) {

    if(cl_funcall(3, list, base, ecl_make_symbol(properties.at(i).c_str(), "CL-USER")) == ECL_T)
      continue;
    
    if(cl_funcall(3, str,
		  base,
		  ecl_make_symbol(properties.at(i).c_str(), "CL-USER")) == ECL_T) {
      auto any = getStringProp(base, properties.at(i));
      QLineEdit *edit = new QLineEdit(QString(any.c_str()), this);

      connect(edit, &QLineEdit::editingFinished,
      	      [=]() { editingStdStringFinished(base, properties.at(i), edit); });

      std::string fieldName = properties.at(i);
      // std::string errors = base->getErrorsOf(fieldName);

      // QLabel *error_field = new QLabel(QString(errors.c_str()), this);
      // error_field->setStyleSheet("QLabel { color: red; }");

      // field_errorlabel_mapping[properties.at(i)] = error_field;
      
      data->addRow(QString(properties.at(i).c_str()), edit);
      // data->addRow(QString(""), error_field);
    }
    else if (cl_funcall(3, prop_bool, base, ecl_make_symbol(properties.at(i).c_str(), "CL-USER")) == ECL_T) {
      bool b = getBoolProp(base, properties.at(i));

      QCheckBox *cb = new QCheckBox(properties.at(i).c_str(), this);
      cb->setCheckState(b ? Qt::Checked: Qt::Unchecked);

      connect(cb, &QCheckBox::stateChanged,
	      [&](int state) { editingBoolFinished(base, properties.at(i), state == Qt::Checked); });

      data->addRow("", cb);
      
    }
    // This probably could be macrofied for all the numeric types
    else if (cl_funcall(3, prop_number, base, ecl_make_symbol(properties.at(i).c_str(), "CL-USER")) == ECL_T) {
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
    else if (cl_funcall(3, prop_float, base, ecl_make_symbol(properties.at(i).c_str(), "CL-USER")) == ECL_T) {
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
    else if(cl_funcall(3, prop_sym, base, ecl_make_symbol(properties.at(i).c_str(), "CL-USER")) == ECL_T) {
      std::string scriptTypeResult = getScriptTypeAsString(base, properties.at(i));
      QLabel *lbl = new QLabel(scriptTypeResult.c_str());
      data->addRow(QString(properties.at(i).c_str()), lbl);
    }
    else if(cl_funcall(3, str, base, ecl_make_symbol(properties.at(i).c_str(), "CL-USER")) == ECL_T) {
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
      // cl_object r = &editorController::instance->document;

      // cl_object currentValue = get(base, properties.at(i).c_str());
      // std::string nameResult;
      // if (currentValue) {
      // 	nameResult = getStringProp(currentValue, "name");
      // }
      // else {
      // 	nameResult = "Empty";
      // }
      
      // std::vector<cl_object>* kids = r->registryOf(type);
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

void Propertyeditor::resetLayout(cl_object base) {
  cl_object r = editorController::instance->document.getValue();
  cl_object regToList = makefn("qmapper.root:root-registryToList");
  auto reg = cl_funcall(2, regToList, r);
  l = new QVBoxLayout;
  
  data = makeLayout(base);

  // data->addRow(QString("Row: "),
  // 	       new QLabel(QString(std::to_string(indexOf<cl_object>(&reg, base)).c_str()), this));
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

Propertyeditor::Propertyeditor(cl_object base, QWidget *parent): QDialog(parent)
{
  cl_object format = makefn("format");
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"base in ctr is ~a ~%\""), base);
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
