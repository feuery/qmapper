#include <editorController_guile.h>
#include <script.h>
#include <files.h>

extern "C" { 

  SCM add_map(SCM s_w, SCM s_h, SCM s_layerCount) {
    int w = scm_to_int(s_w),
      h = scm_to_int(s_h),
      layers = scm_to_int(s_layerCount);

    int count = editorController::instance->document.registrySize();

    Mapcontainer *mc = new Mapcontainer(w, h, layers, &editorController::instance->document);
    mc->setName(QString("Map %1").arg(count).toStdString());

    editorController::instance->documentTreeModel->begin(count);
    (*editorController::instance->document.getMaps())[mc->getId()] = mc;
    editorController::instance->documentTreeModel->end();
  
    return SCM_BOOL_T;
  }

  SCM add_layer(SCM map_index)
  {
    int map_i = scm_to_int(map_index);
    Rootcontainer &r = static_cast<Rootcontainer&>(editorController::instance->document);
    
    Map *m =  r.nth<Map>(map_i);
    int c = m->getLayers()->size();
    
    // map *m = editorController::instance->document->all_maps->at(map_i);

    editorController::instance->documentTreeModel->beginMap(map_i);

    Layercontainer *l = new Layercontainer(m->width(), m->height());
    l->set_parent(m);
    l->setName(QString("Layer %1").arg(c).toStdString());
    m->getLayers()->push_back(l);

    editorController::instance->documentTreeModel->end();

    return SCM_BOOL_T;
    
  }

  SCM delete_layer(SCM map_index_scm, SCM layer_index_scm)
  {
    int map_index = scm_to_int(map_index_scm),
      layer_index = scm_to_int(layer_index_scm);

    Rootcontainer &r = static_cast<Rootcontainer&>(editorController::instance->document);
    
    if(! (map_index >= 0 && map_index < r.registrySize())) return SCM_BOOL_F;

    editorController::instance->documentTreeModel->beginMap(map_index);
    std::vector<Layer*>* layerset = r.nth<Map>(map_index)->getLayers();

    if(! (layer_index >= 0 && layer_index < layerset->size())) return SCM_BOOL_F;

    layerset->erase(layerset->begin() + layer_index);
    editorController::instance->documentTreeModel->end();

    return SCM_BOOL_T;
  }

  std::string genNs() {
    return std::string("user") + std::to_string(rand() % 1000000);
  }

  SCM add_glsl_script()
  {
    editorController::instance->documentTreeModel->begin();

    Script *scrpt = new Script;
    scrpt->setScript_type(glsl);
    scrpt->setName("A new GLSL script");
    std::string ns = genNs();
    scrpt->setNs(ns);
    
    (*editorController::instance->document.getScripts())[scrpt->getId()] = scrpt;
    editorController::instance->documentTreeModel->end();

    return SCM_BOOL_T;
  }

  SCM add_scheme_script()
  {
    editorController::instance->documentTreeModel->begin();

    Script *scrpt = new Script;
    scrpt->setScript_type(scheme);
    scrpt->setName("A new Scheme script");
    std::string ns = genNs();
    scrpt->setNs(ns);
    
    (*editorController::instance->document.getScripts())[scrpt->getId()] = scrpt;
    editorController::instance->documentTreeModel->end();

    return SCM_BOOL_T;
  }

  SCM resize_current_map(SCM w, SCM h, SCM horizontal_anchor, SCM vertical_anchor)
  {
    editorController *ec = editorController::instance;

    // It might cause problems if we screw with data structures while renderer is deep within the same datastructures
    bool oldRenderingState = ec->renderingEnabled;
    ec->renderingEnabled = false;
    
    Map *m = toMap(ec->document.fetchRegister("Map", ec->indexOfChosenMap));
    int new_w = scm_to_int(w),
      new_h = scm_to_int(h);
    const char* h_a = scm_to_locale_string(scm_symbol_to_string(horizontal_anchor));
    std::string horizontal_a = h_a;

    const char *v_a = scm_to_locale_string(scm_symbol_to_string(vertical_anchor));
    std::string vertical_a = v_a;

    qDebug() << "Vertical anchor: " << v_a;

    m->resize(new_w, new_h, vertical_a == "TOP"? TOP: BOTTOM, horizontal_a == "RIGHT"? RIGHT: LEFT);

    ec->renderingEnabled = oldRenderingState;;

    return SCM_BOOL_T;
  }

  SCM toggle_rendering() {
    editorController *ec = editorController::instance;
    qDebug() << (ec->renderingEnabled? "Stopping renderer": "Starting renderer");
    ec->renderingEnabled = !ec->renderingEnabled;
    return SCM_BOOL_T;
  }

  SCM print_json(SCM type, SCM id)
  {
    const char *c_id = scm_to_locale_string(id),
      *c_type = scm_to_locale_string(type);
    std::string cc_id = c_id,
      cc_type = c_type;
    editorController *ec = editorController::instance;

    Propertierbase *b = ec->document.fetchRegister(cc_type, cc_id);
    qDebug() << "Id " << c_id << "'s (" << b->type_identifier().c_str() << ") json: " << b->toJSON().c_str();
    return SCM_BOOL_T;
  }

  SCM load_sprite(SCM s_path) {
    const char *path = scm_to_locale_string(s_path);
    editorController::instance->map_view->glLambdas.enqueue([=]() {
	editorController::instance->loadSprite(path);
      });
    return SCM_BOOL_T;
  }

  SCM load_animation(SCM s_path, SCM s_framecount, SCM s_frameLifeTime)
  {
    const char *path = scm_to_locale_string(s_path);
    int framecount = scm_to_int(s_framecount),
      frameLifeTime = scm_to_int(s_frameLifeTime);

    editorController::instance->map_view->glLambdas.enqueue([=]() {
	editorController::instance->loadAnimation(path, framecount, frameLifeTime);
      });

    return SCM_BOOL_T;
  }

  SCM keyDown(SCM key) {
    Qt::Key k = scm_qt_key_pairs[key];
    return editorController::instance->keyMap[k]? SCM_BOOL_T: SCM_BOOL_F;
  }

  SCM getMouse() {
    auto ec = editorController::instance;
    SCM list = scm_list_2(scm_from_int(ec->mouseX), scm_from_int(ec->mouseY));
    return list;
  }

  SCM getMouseButtonState() {
    auto ec = editorController::instance;

    auto btns = QGuiApplication::mouseButtons();
    bool left = btns & Qt::LeftButton,
      right = btns & Qt::RightButton;

    return scm_list_2(scm_from_bool(left), scm_from_bool(right));
  }

  #define returnResult const char *res = result.c_str();\
    return scm_from_locale_string(res);

  SCM getProp(SCM id, SCM type, SCM propname) {
    auto ec = editorController::instance;

    const char *c_type = scm_to_locale_string(type),
      *c_prop = scm_to_locale_string(propname),
      *c_id = scm_to_locale_string(id);
    
    std::string cc_type = c_type;
    bool lol = false;

    Propertierbase *obj = ec->document.fetchRegister(c_type, c_id);
    std::string proptype = obj->type_name(c_prop);

    std::string result = "";
    if(proptype == "std::string") {
      result = obj->get(c_prop, &lol, result);
      returnResult;
    }
    else if (proptype == "int") {
      int i = 42;

      return scm_from_int(obj->get(c_prop, &lol, i));
    }
    else if (proptype == "bool") {
      return scm_from_bool(obj->get(c_prop, &lol, lol));
    }
    else if (proptype == "unsigned char") {
      unsigned char trol = -1;
      int result = obj->get(c_prop, &lol, trol);
      return scm_from_int(result);
    }
    else if (proptype == "Script*") {
      Script* scr;
      scr = obj->get(c_prop, &lol, scr);
      result = scr->getContents();
      returnResult;
    }
    else if(proptype == "scriptTypes") {
      scriptTypes t;
      t = obj->get(c_prop, &lol, t);
      return scm_string_to_symbol(scm_from_locale_string(t == glsl? "glsl" : "scheme"));
    }
    else if(proptype == "float") {
      float val = obj->get(c_prop, &lol, 2.2f);
      return scm_from_double(val);
    }

    qDebug() << "Couldn't find a valid value with params" << c_id << ", " << c_type << ", " << c_prop << " - and with type " << proptype.c_str();

    return SCM_BOOL_F;
  }

  #define setVal obj->set(c_prop, val)

  SCM setProp(SCM id, SCM type, SCM propname, SCM value)
  {
    auto ec = editorController::instance;

    const char *c_type = scm_to_locale_string(type),
      *c_prop = scm_to_locale_string(propname),
      *c_id = scm_to_locale_string(id);
    
    std::string cc_type = c_type;
    bool lol = false;

    Propertierbase *obj = ec->document.fetchRegister(c_type, c_id);
    std::string proptype = obj->type_name(c_prop);

    std::string result = "";

    ec->documentTreeModel->begin();
    
    if(proptype == "std::string") {
      std::string val = scm_to_locale_string(value);
      setVal;
    }
    else if (proptype == "int") {
      int val = scm_to_int(value);
      setVal;
    }
    else if (proptype == "bool") {
      bool val = scm_to_bool(value);
      setVal;
    }
    else if (proptype == "unsigned char") {
      int lol = scm_to_int(value);
      unsigned char val = static_cast<unsigned char>(lol);
      setVal;
    }
    else if (proptype == "Script*") {
      Script* scr;
      scr = obj->get(c_prop, &lol, scr);

      const char *content = scm_to_locale_string(value);
      scr->setContents(content);
    }
    else if(proptype == "scriptTypes") {
      const char *result = scm_to_locale_string(scm_symbol_to_string(value));
      std::string r = result;
      scriptTypes val = r == "glsl"? glsl: scheme;
      setVal;      
    }
    else if(proptype == "float") {
      float val = scm_to_double(value);
      setVal;
    }
    else
      qDebug() << "Couldn't find a valid value with params" << c_id << ", " << c_type << ", " << c_prop << " - and with type " << proptype.c_str();
    ec->documentTreeModel->end();

    return SCM_BOOL_T;
  }

  SCM addEvent(SCM id, SCM type, SCM propname, SCM lambda)
  {
    auto ec = editorController::instance;

    const char *c_type = scm_to_locale_string(type),
      *c_prop = scm_to_locale_string(propname),
      *c_id = scm_to_locale_string(id);

    std::string _prop = c_prop;
    
    std::string cc_type = c_type;
    bool lol = false;

    Propertierbase *obj = ec->document.fetchRegister(c_type, c_id);
    FUN f;
    f.call_guile = true;
    f.guile = lambda;

    obj->addEvent(_prop, f);
    return SCM_BOOL_T;
  }

  SCM qscm_puts(SCM str)
  {
    puts(scm_to_locale_string(str));
    return SCM_BOOL_T;
  }

  SCM render_obj(SCM obj_ptr) {
    void *ptr = scm_to_pointer(obj_ptr);
    obj* real_obj_ptr = static_cast<obj*>(ptr);
    real_obj_ptr->render();
  }

  SCM MsTime() {
    return scm_from_long(time(nullptr) * 1000);
  }
}
