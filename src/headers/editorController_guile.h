#ifndef EDITORCONTROLLER_GUILE_H
#define EDITORCONTROLLER_GUILE_H

#include <libguile.h>
#include <editorController.h>
#include <mapContainer.h>

extern "C" {
  SCM add_map(SCM s_w, SCM s_h, SCM s_layerCount);
  SCM add_layer(SCM map_index);
  SCM delete_root_index(SCM index, SCM type_name);
  SCM delete_layer(SCM map_index, SCM layer_index);
  SCM add_glsl_script();
  SCM add_scheme_script();
  SCM resize_current_map(SCM w, SCM h, SCM horizontal_anchor, SCM vertical_anchor);
  SCM toggle_rendering();
}

#endif //EDITORCONTROLLER_GUILE_H
