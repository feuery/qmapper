#include <editorController_guile.h>
#include <files.h>
#include <guile_fn.h>

extern "C" { 

  cl_object add_map(cl_object w, cl_object h, cl_object layerCount) {
    static cl_object makeMap   = ecl_make_symbol("make-map-with-layers", "CL-USER");
    static cl_object pushMap   = ecl_make_symbol("push-selected-map", "CL-USER");
    static cl_object registrySize = ecl_make_symbol("root-registrySize", "CL-USER");
   
    cl_object count = cl_funcall(2, registrySize,
			   editorController::instance->document);

    cl_object m = cl_funcall(5, makeMap,
			     c_string_to_object((std::to_string(fixint(count)+1)+"th map").c_str()),
		       w,
		       h,
		       layerCount);

    editorController::instance->document = cl_funcall(3, pushMap,
						      editorController::instance->document,
						      m);
    return ECL_T;
  }

  cl_object add_layer(cl_object map_index)
  {
    cl_object addLayer = ecl_make_symbol("add-layer", "CL-USER");
    
    editorController::instance->documentTreeModel->begin();
    editorController::instance->document = cl_funcall(3, addLayer,
						      editorController::instance->document,
						      map_index);
    
    editorController::instance->documentTreeModel->end();

    return ECL_T;
    
  }

  cl_object delete_layer(cl_object map_index_scm, cl_object layer_index_scm)
  {
    static cl_object deleteLayer = ecl_make_symbol("delete-layer", "CL-USER");
    editorController::instance->documentTreeModel->begin();
    editorController::instance->document = cl_funcall(4, deleteLayer,
						      editorController::instance->document,
						      map_index_scm,
						      layer_index_scm);
    editorController::instance->documentTreeModel->end();
    return ECL_T;
  }

  std::string genNs() {
    return std::string("user") + std::to_string(rand() % 1000000);
  }

  cl_object add_glsl_script()
  {
    static cl_object makeScript = ecl_make_symbol("make-Script", "CL-USER");
    static cl_object pushScript = ecl_make_symbol("push-script", "CL-USER");
    
    editorController::instance->documentTreeModel->begin();

    cl_object scrpt = cl_funcall(5, makeScript,
			   c_string_to_object(""),
			   c_string_to_object("A new GLSL script"),
			   c_string_to_object(genNs().c_str()),
				 ecl_make_symbol("glsl", "CL-USER"));
    editorController::instance->document = cl_funcall(3, pushScript,
						      editorController::instance->document,
						      scrpt);
    editorController::instance->documentTreeModel->end();

    return ECL_T;
  }

  cl_object add_scheme_script()
  {
    static cl_object makeScript = ecl_make_symbol("make-Script", "CL-USER");
    static cl_object pushScript = ecl_make_symbol("push-script", "CL-USER");
    
    editorController::instance->documentTreeModel->begin();

    cl_object scrpt = cl_funcall(5, makeScript,
			   c_string_to_object(""),
			   c_string_to_object("A new Scheme script"),
			   c_string_to_object(genNs().c_str()),
				 ecl_make_symbol("scheme", "CL-USER"));
    editorController::instance->document = cl_funcall(3, pushScript,
						      editorController::instance->document,
						      scrpt);
    editorController::instance->documentTreeModel->end();

    return ECL_T;
  }

  cl_object resize_current_map(cl_object w, cl_object h, cl_object horizontal_anchor, cl_object vertical_anchor)
  {
    editorController *ec = editorController::instance;

    // It might cause problems if we screw with data structures while renderer is deep within the same datastructures
    bool oldRenderingState = ec->renderingEnabled;
    ec->renderingEnabled = false;

    static cl_object selectedMap = ecl_make_symbol("root-chosenMap", "CL-USER");
    static cl_object push_selectedMap = ecl_make_symbol("push-selected-map", "CL-USER");
    static cl_object map_resize = ecl_make_symbol("Map-resize", "CL-USER");
    
    // Map *m = toMap(ec->document.fetchRegister("Map", ec->indexOfChosenMap));
    cl_object m = cl_funcall(2, selectedMap, editorController::instance->document);

    std::string horizontal_a(ecl_string_to_string(ecl_symbol_name(horizontal_anchor)));
    std::string vertical_a(ecl_string_to_string(ecl_symbol_name(vertical_anchor)));

    m = cl_funcall(6, map_resize,
		   m,
		   w,
		   h,
		   vertical_anchor,
		   horizontal_anchor);
		   
    editorController::instance->document = cl_funcall(3, push_selectedMap,
						      editorController::instance->document,
						      m);
						      

    ec->renderingEnabled = oldRenderingState;;

    return ECL_T;
  }

  cl_object toggle_rendering() {
    editorController *ec = editorController::instance;
    qDebug() << (ec->renderingEnabled? "Stopping renderer": "Starting renderer");
    ec->renderingEnabled = !ec->renderingEnabled;
    return ECL_T;
  }

  // cl_object print_json(cl_object type, cl_object id)
  // {
  //   const char *c_id = scm_to_utf8_string(id),
  //     *c_type = scm_to_utf8_string(type);
  //   std::string cc_id = c_id,
  //     cc_type = c_type;
  //   editorController *ec = editorController::instance;

  //   Propertierbase *b = ec->document.fetchRegister(cc_type, cc_id);
  //   qDebug() << "Id " << c_id << "'s (" << b->type_identifier().c_str() << ") json: " << b->toJSON().c_str();
  //   return ECL_T;
  // }

  cl_object load_sprite(cl_object s_path) {
    puts("TODO implement sprites");
    // const char *path = scm_to_utf8_string(s_path);
    // editorController::instance->map_view->glLambdas.enqueue([=]() {
    // 	editorController::instance->loadSprite(path);
    //   });
    return ECL_NIL;
  }

  cl_object load_animation(cl_object s_path, cl_object s_framecount, cl_object s_frameLifeTime)
  {
    // const char *path = scm_to_utf8_string(s_path);
    // int framecount = fixint(s_framecount),
    //   frameLifeTime = fixint(s_frameLifeTime);

    // editorController::instance->map_view->glLambdas.enqueue([=]() {
    // 	editorController::instance->loadAnimation(path, framecount, frameLifeTime);
    //   });

    puts("TODO implement animations");
    return ECL_NIL;
  }

  cl_object keyDown(cl_object key) {
    puts("TODO FIX");
    return ECL_T;
    // Qt::Key k = scm_qt_key_pairs[key];
    // return editorController::instance->keyMap[k]? ECL_T: ECL_NIL;
  }

  cl_object getMouse() {
    auto ec = editorController::instance;
    cl_object list = cl_list(2, ecl_make_fixnum(ec->mouseX), ecl_make_fixnum(ec->mouseY));
    return list;
  }

  cl_object getMouseButtonState() {
    auto ec = editorController::instance;

    auto btns = QGuiApplication::mouseButtons();
    bool left = btns & Qt::LeftButton,
      right = btns & Qt::RightButton;

    return cl_list(2, ecl_make_bool(left), ecl_make_bool(right));
  }

  #define returnResult const char *res = result.c_str();\
    return c_string_to_object(res);

  // cl_object getProp(cl_object id, cl_object type, cl_object propname) {
  //   auto ec = editorController::instance;

  //   const char *c_type = scm_to_utf8_string(type),
  //     *c_prop = scm_to_utf8_string(propname),
  //     *c_id = scm_to_utf8_string(id);
    
  //   std::string cc_type = c_type;
  //   bool lol = false;

  //   Propertierbase *obj = ec->document.fetchRegister(c_type, c_id);
  //   std::string proptype = obj->type_name(c_prop);

  //   std::string result = "";
  //   if(proptype == "std::string") {
  //     result = obj->get(c_prop, &lol, result);
  //     returnResult;
  //   }
  //   else if (proptype == "int") {
  //     int i = 42;

  //     return ecl_make_fixnum(obj->get(c_prop, &lol, i));
  //   }
  //   else if (proptype == "bool") {
  //     return scm_from_bool(obj->get(c_prop, &lol, lol));
  //   }
  //   else if (proptype == "unsigned char") {
  //     unsigned char trol = -1;
  //     int result = obj->get(c_prop, &lol, trol);
  //     return ecl_make_fixnum(result);
  //   }
  //   else if (proptype == "Script*") {
  //     Script* scr;
  //     scr = obj->get(c_prop, &lol, scr);
  //     result = scr->getContents();
  //     returnResult;
  //   }
  //   else if(proptype == "scriptTypes") {
  //     scriptTypes t;
  //     t = obj->get(c_prop, &lol, t);
  //     return scm_string_to_symbol(c_string_to_object(t == glsl? "glsl" : "scheme"));
  //   }
  //   else if(proptype == "float") {
  //     float val = obj->get(c_prop, &lol, 2.2f);
  //     return scm_from_double(val);
  //   }

  //   qDebug() << "Couldn't find a valid value with params" << c_id << ", " << c_type << ", " << c_prop << " - and with type " << proptype.c_str();

  //   return ECL_NIL;
  // }

  // #define setVal obj->set(c_prop, val)

  // cl_object setProp(cl_object id, cl_object type, cl_object propname, cl_object value)
  // {
  //   auto ec = editorController::instance;

  //   const char *c_type = scm_to_utf8_string(type),
  //     *c_prop = scm_to_utf8_string(propname),
  //     *c_id = scm_to_utf8_string(id);
    
  //   std::string cc_type = c_type;
  //   bool lol = false;

  //   Propertierbase *obj = ec->document.fetchRegister(c_type, c_id);
  //   std::string proptype = obj->type_name(c_prop);

  //   std::string result = "";

  //   ec->documentTreeModel->begin();
    
  //   if(proptype == "std::string") {
  //     std::string val = scm_to_utf8_string(value);
  //     setVal;
  //   }
  //   else if (proptype == "int") {
  //     int val = fixint(value);
  //     setVal;
  //   }
  //   else if (proptype == "bool") {
  //     bool val = scm_to_bool(value);
  //     setVal;
  //   }
  //   else if (proptype == "unsigned char") {
  //     int lol = fixint(value);
  //     unsigned char val = static_cast<unsigned char>(lol);
  //     setVal;
  //   }
  //   else if (proptype == "Script*") {
  //     Script* scr;
  //     scr = obj->get(c_prop, &lol, scr);

  //     const char *content = scm_to_utf8_string(value);
  //     scr->setContents(content);
  //   }
  //   else if(proptype == "scriptTypes") {
  //     const char *result = ecl_make_symbol(value));
  //     std::string r = result;
  //     scriptTypes val = r == "glsl"? glsl: scheme;
  //     setVal;      
  //   }
  //   else if(proptype == "float") {
  //     float val = scm_to_double(value);
  //     setVal;
  //   }
  //   else
  //     qDebug() << "Couldn't find a valid value with params" << c_id << ", " << c_type << ", " << c_prop << " - and with type " << proptype.c_str();
  //   ec->documentTreeModel->end();

  //   return ECL_T;
  // }

  cl_object addEvent(cl_object id, cl_object type, cl_object propname, cl_object lambda)
  {
    puts("TODO implement events in pure scheme");
    // auto ec = editorController::instance;

    // const char *c_type = scm_to_utf8_string(type),
    //   *c_prop = scm_to_utf8_string(propname),
    //   *c_id = scm_to_utf8_string(id);

    // std::string _prop = c_prop;
    
    // std::string cc_type = c_type;
    // bool lol = false;

    // Propertierbase *obj = ec->document.fetchRegister(c_type, c_id);
    // FUN f;
    // f.call_guile = true;
    // f.guile = lambda;

    // obj->addEvent(_prop, f);
    return ECL_NIL;
  }

  cl_object qscm_puts(cl_object str)
  {
    puts(ecl_string_to_string(str).c_str());
    return ECL_T;
  }

  // cl_object render_obj(cl_object obj_ptr) {
  //   void *ptr = scm_to_pointer(obj_ptr);
  //   obj* real_obj_ptr = static_cast<obj*>(ptr);
  //   real_obj_ptr->render();
  // }

  cl_object MsTime() {
    return ecl_make_long(time(nullptr) * 1000);
  }

  static std::unordered_map<std::string, cl_object> fn_cache;

  cl_object register_fn(cl_object name, cl_object fn) {
    std::string n = ecl_string_to_string(name);
    fn_cache[n] = fn;
    printf("Registered fn %s\n", n.c_str());
    return ECL_T;
  }

  // cl_object get_fn(const char *name) {
  //   return fn_cache.at(std::string(name));
  // }
}
