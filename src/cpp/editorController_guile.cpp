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
}
