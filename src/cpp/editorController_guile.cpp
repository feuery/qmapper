#include <editorController_guile.h>
#include <script.h>

extern "C" {

  SCM add_map(SCM s_w, SCM s_h, SCM s_layerCount) {
    int w = scm_to_int(s_w),
      h = scm_to_int(s_h),
      layers = scm_to_int(s_layerCount);

    int count = editorController::instance->document.registry->size();

    Mapcontainer *mc = new Mapcontainer(w, h, layers, &editorController::instance->document);
    mc->setName(QString("Map %1").arg(count).toStdString());

    editorController::instance->documentTreeModel->begin(count);
    (*editorController::instance->document.registry)[mc->getId()] = mc;
    editorController::instance->documentTreeModel->end();
  
    return SCM_BOOL_T;
  }

  SCM add_layer(SCM map_index)
  {
    int map_i = scm_to_int(map_index);
    root &r = editorController::instance->document;
    auto map_strIndex = r.indexOf(map_i);
    
    Map *m =  toMap(r.registry->at(map_strIndex));
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

  SCM delete_root_index(SCM index_scm)
  {
    int index = scm_to_int(index_scm);
    root &r = editorController::instance->document;
    if(! (index >= 0 && index < r.registry->size())) return SCM_BOOL_F;
    editorController::instance->documentTreeModel->begin(index);

    auto strInd = r.indexOf(index);
    
    r.registry->erase(strInd);
    editorController::instance->documentTreeModel->end();
    return SCM_BOOL_T;
  }

  SCM delete_layer(SCM map_index_scm, SCM layer_index_scm)
  {
    int map_index = scm_to_int(map_index_scm),
      layer_index = scm_to_int(layer_index_scm);

    root &r = editorController::instance->document;
    
    if(! (map_index >= 0 && map_index < r.registry->size())) return SCM_BOOL_F;

    auto map_ind_str = r.indexOf(map_index);

    editorController::instance->documentTreeModel->beginMap(map_index);
    std::vector<Layer*>* layerset = toMap(r.registry->at(map_ind_str))->layers;

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
    
    (*editorController::instance->document.registry)[scrpt->getId()] = scrpt;
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
    
    (*editorController::instance->document.registry)[scrpt->getId()] = scrpt;
    editorController::instance->documentTreeModel->end();

    return SCM_BOOL_T;
  }
}
