#include <map_script_form.h>
#include <editorController.h>
void map_script_form::buildUI(const char *map_id) {
  ui.setupUi(this);
  ui.gridLayout->setContentsMargins(0,0,0,0);

  cl_object get_all_scripts = makefn("qmapper.root:get-all-scripts"),
    map = get(get(editorController::instance->document.getValue(), "MAPS"), map_id),
    to_list = makefn("qmapper.std:to-list"),
    map_scripts = cl_funcall(2, to_list, get(map, "SCRIPTS-TO-RUN")),
    len_fn = makefn("length"),
    map_scripts_len = cl_funcall(2, len_fn, map_scripts),
    drop_seq = makefn("qmapper.script:drop-script-with-ids"),
    scripts = cl_funcall(2, get_all_scripts, editorController::instance->document.getValue()),
    nth = makefn("nth"),
    is_lisp = makefn("qmapper.script:script-is-lisp?"),
    id_to_ns = makefn("qmapper.root:script-id->ns");

  scripts = cl_funcall(2, to_list,
		       cl_funcall(3, drop_seq, scripts, map_scripts));

  cl_object len = cl_funcall(2, len_fn, scripts);

  
  for(int i = 0; i < ecl_to_int(len); i++) {
    cl_object script = cl_funcall(3, nth, ecl_make_int(i), scripts),
      format = makefn("format");
    cl_funcall(4, format, ECL_T, c_string_to_object("\"script is ~a~%\""), script);
    if(script == ECL_NIL) {
      printf ("kohdassa %d ", i);
      cl_funcall(4, format, ECL_T, c_string_to_object("\"script is nil, scripts: ~a~%\""), scripts);
      throw "";
    }
    std::string ns = ecl_string_to_string(get(script, "NS"));
    
    printf("found ns %s\n", ns.c_str());

    if(cl_funcall(2, is_lisp, script) == ECL_T) {
      ui.comboBox->addItem(QString(ns.c_str()));
    }
  }

  for(int i = 0; i < ecl_to_int(map_scripts_len); i++) {
    cl_object script_id = cl_funcall(3, nth, ecl_make_int(i), map_scripts),
      cl_ns = cl_funcall(3, id_to_ns,
			 editorController::instance->document.getValue(),
			 script_id);
    
    std::string ns = ecl_string_to_string(cl_ns);
    
    printf("found ns %s from map\n", ns.c_str());
    ui.listWidget->addItem(QString(ns.c_str()));
  }
  
  connect(ui.buttonBox, &QDialogButtonBox::accepted, [&]() {
						       close();
						     });
  std::string c_map_id(map_id);
  printf("c_map_id on %s\n", c_map_id.c_str());
  connect(ui.btn_Add, &QPushButton::clicked, [=]() {
					       QString ns = ui.comboBox->currentText();
					       ui.listWidget->addItem(ns);
					       ui.comboBox->setCurrentText("");

					       const char *ns_c = ("\""+ns.toStdString()+"\"").c_str();
					       printf("c-map-id %s\n", c_map_id.c_str());
					       
					       cl_object cl_map_id = c_string_to_object(c_map_id.c_str()),
						 add_script_to_map = makefn("qmapper.root:add-script-to-map"),
						 format = makefn("format"),
						 script_ns = c_string_to_object(ns_c);

					       cl_funcall(4, format, ECL_T, c_string_to_object("\"Miltä map-id näyttää jossain välitilassa? ~a~%\""), cl_map_id);

					       cl_object new_doc = cl_funcall(4, add_script_to_map,
									      editorController::instance->document.getValue(),
									      cl_map_id,
									      script_ns);
					       editorController::instance->document.setValue(new_doc);
					       
					       int index = ui.comboBox->findText(ns);
					       if(index >= 0) ui.comboBox->removeItem(index);

					     });

  connect(ui.btnDelete, &QPushButton::clicked, [=]() {
						 QString ns = ui.listWidget->currentItem()->text();
						 const char *ns_c = ("\""+ns.toStdString()+"\"").c_str();

						 cl_object drop_script = makefn("qmapper.root:drop-script-from-map"),
						   cl_map_id = c_string_to_object(c_map_id.c_str()),
						   script_ns = c_string_to_object(ns_c),
						   new_doc = cl_funcall(4, drop_script,
									editorController::instance->document.getValue(),
									cl_map_id,
									script_ns);
						 
						 editorController::instance->document.setValue(new_doc);
						 ui.comboBox->addItem(ns);
						 auto item = ui.listWidget->currentItem();
						 delete item;
					       });
}

map_script_form::map_script_form(const char *map_id) {
  buildUI(map_id);
}
