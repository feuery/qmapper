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
    editorController::instance->document.doRegister("Map", mc->getId(), mc);
    editorController::instance->documentTreeModel->end();
  
    return SCM_BOOL_T;
  }

  SCM add_layer(SCM map_index)
  {
    int map_i = scm_to_int(map_index);
    Rootcontainer &r = static_cast<Rootcontainer&>(editorController::instance->document);
    
    Map *m =  r.nth<Map>(std::string("Map"), map_i);
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

  SCM delete_root_index(SCM index_scm, SCM type_name)
  {
    char *type = scm_to_locale_string(type_name);
    std::string tyype = type;
    int index = scm_to_int(index_scm);
    root &r = editorController::instance->document;
    if(! (index >= 0 && index < r.typeRegistrySize(tyype))) return SCM_BOOL_F;
    editorController::instance->documentTreeModel->begin(index);
    
    r.erase(tyype, index);
    editorController::instance->documentTreeModel->end();

    free(type);
    return SCM_BOOL_T;
  }

  SCM delete_layer(SCM map_index_scm, SCM layer_index_scm)
  {
    int map_index = scm_to_int(map_index_scm),
      layer_index = scm_to_int(layer_index_scm);

    Rootcontainer &r = static_cast<Rootcontainer&>(editorController::instance->document);
    
    if(! (map_index >= 0 && map_index < r.registrySize())) return SCM_BOOL_F;

    editorController::instance->documentTreeModel->beginMap(map_index);
    std::vector<Layer*>* layerset = r.nth<Map>("Map", map_index)->getLayers();

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
    
    editorController::instance->document.doRegister("Script", scrpt->getId(), scrpt);
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
    
    editorController::instance->document.doRegister("Script", scrpt->getId(), scrpt);
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

  SCM print_json(SCM row)
  {
    int row_i = scm_to_int(row);
    editorController *ec = editorController::instance;

    Propertierbase *b = ec->document.registryToList({}).at(row_i);
    qDebug() << "Row " << row_i << "'s (" << b->type_identifier().c_str() << ") json: " << b->toJSON().c_str();
    return SCM_BOOL_T;
  }

  SCM g_from_json(SCM type_name, SCM s_row)
  {
    toggle_rendering();
    const char *type_cstr = scm_to_locale_string(type_name);
    std::string type = type_cstr;
    int row = scm_to_int(s_row);
    
    if (type == "Layer") {
      
    }
    else if (type == "Map") {
      Propertierbase *b = editorController::instance->document.registryToList().at(row);
      Map *m = new Mapcontainer;
      const char *lol = b->toJSON().c_str();
      m->fromJSON(lol);
      std::string other_js = b->toJSON();
      
      if (m->toJSON() == other_js) {
	qDebug() << "Maps do match";
      }
      else {
	qDebug () << "Maps don't match";
	qDebug() << "Loaded json: " << m->toJSON().c_str();
	qDebug() << "Documented json: " << other_js.c_str();
      }
      
    }
    else if (type == "root") {
      
    }
    else if (type == "Script") {
      
    }
    else if (type == "Tile") {
      
    }
    else if (type == "Tileset") {
      
    }
    else qDebug() << "Didn't recognize type " << type_cstr;
    toggle_rendering();

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
}
