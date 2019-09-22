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


bool has_meta(cl_object field_meta, const char *meta_value)
{
  QString mval = meta_value;
  mval = "\""+mval.toUpper()+"\"";
  cl_object member = lisp("(lambda (field-meta) (let ((field-meta (fset:convert 'list field-meta)))  (member " + mval.toStdString() + " field-meta :test #'string=)))"),
    result = cl_funcall(2, member, field_meta);
  return result != ECL_NIL;
}

Propertyeditor::Propertyeditor(cl_object list_path_to_object, QWidget *parent): QDialog(parent)
{
  cl_object get_in = makefn("qmapper.std:get-prop-in"),
    root = editorController::instance->document.getValue(),
    base = cl_funcall(3, get_in, root, list_path_to_object),
    prin = makefn("prin1-to-string"),
    obj_str = cl_funcall(2, prin, base),
    // tää on formaatissa ("MAPS" "ID" "LOL") eikä esim #["MAPS" "ID" "LOL"] :)))))
    path_str = cl_funcall(2, prin, list_path_to_object);

  std::string c_obj_str = ecl_string_to_string(obj_str),
    path = ecl_string_to_string(path_str);
  printf("c_obj_str: %s\n", c_obj_str.c_str());
  printf("Path: %s\n", path.c_str());

  assert(base != ECL_NIL);
  
  ui.setupUi(this);
  ui.lbl_path->setText(("Path to obj: " + path).c_str());
  ui.txtLisp->setPlainText(c_obj_str.c_str());

  connect(ui.buttonBox, &QDialogButtonBox::accepted, [=]() {
							 merge_meta = makefn("qmapper.root:merge-meta"),
							 inner_path = lisp(path),
							 new_obj = cl_funcall(3, merge_meta, lisp(lisp_code), inner_path),
							 validator_fetcher = lisp("(lambda (type-name) (qmapper.std:get-prop qmapper.std:validators type-name))"),
							 validator = cl_funcall(2, validator_fetcher, c_string_to_object(("\""+c_type_name+"\"").c_str())),
							 validation_result = cl_funcall(2, validator, new_obj);
						       
						       if(validation_result == ECL_NIL) {
							 puts("Validation failed on the lisp side");
						       }
						       else {
							 auto doc = editorController::instance->document;
							 // (format t \"set-propping doc, path ~a and obj ~a~%\" path obj)
							 
							 doc.setValue(cl_funcall(4, 
										 lisp("(lambda (doc path obj) (qmapper.std:set-prop-in doc path obj))"),
										 doc.getValue(),
										 inner_path,
										 new_obj));
							 
						       cl_funcall(4, format, ECL_T, c_string_to_object("\"evaluated: ~a~%\""), lisp(lisp_code));
						       close();
						     });
    
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
