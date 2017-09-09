#include <editorController_guile.h>

extern "C" {

  SCM add_map(SCM s_w, SCM s_h, SCM s_layerCount) {
    int w = scm_to_int(s_w),
      h = scm_to_int(s_h),
      layers = scm_to_int(s_layerCount);

    int count = editorController::instance->document.all_maps->size();

    Mapcontainer *mc = new Mapcontainer(w, h, layers, &editorController::instance->document);
    mc->setName(QString("Map %1").arg(count).toStdString());

    editorController::instance->documentTreeModel->begin(count);
    editorController::instance->document.all_maps->push_back(mc);
    editorController::instance->documentTreeModel->end();
  
    return SCM_BOOL_T;
  }

  SCM add_layer(SCM map_index)
  {
    int map_i = scm_to_int(map_index);
    map *m = editorController::instance->document.all_maps->at(map_i);
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
}
