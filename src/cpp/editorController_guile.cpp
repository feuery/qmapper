#include <editorController_guile.h>
#include <script.h>

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
    int c = m->layers->size();
    
    // map *m = editorController::instance->document->all_maps->at(map_i);

    editorController::instance->documentTreeModel->beginMap(map_i);

    Layercontainer *l = new Layercontainer(m->width(), m->height());
    l->set_parent(m);
    l->setName(QString("Layer %1").arg(c).toStdString());
    m->layers->push_back(l);

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
    std::vector<Layer*>* layerset = r.nth<Map>("Map", map_index)->layers;

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
}
