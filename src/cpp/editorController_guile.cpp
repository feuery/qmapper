#include <editorController_guile.h>

extern "C" {

  SCM add_map(SCM s_w, SCM s_h, SCM s_layerCount) {
  int w = scm_to_int(s_w),
    h = scm_to_int(s_h),
    layers = scm_to_int(s_layerCount);

  Mapcontainer *mc = new Mapcontainer(w, h, layers, &editorController::instance->document);

  editorController::instance->document.all_maps->push_back(mc);
  
  return SCM_BOOL_T;
}
}
