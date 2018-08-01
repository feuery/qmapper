#ifndef EDITORCONTROLLER_GUILE_H
#define EDITORCONTROLLER_GUILE_H

#include <libguile.h>
#include <editorController.h>
#include <guile_qt_keymapping.h>

extern "C" {
  SCM add_map(SCM s_w, SCM s_h, SCM s_layerCount);
  SCM add_layer(SCM map_index);
  SCM delete_layer(SCM map_index, SCM layer_index);
  SCM add_glsl_script();
  SCM add_scheme_script();
  SCM resize_current_map(SCM w, SCM h, SCM horizontal_anchor, SCM vertical_anchor);
  SCM toggle_rendering();

  SCM load_sprite(SCM s_path);
  SCM load_animation(SCM s_path, SCM s_framecount, SCM s_frameLifeTime);

  SCM keyDown(SCM key);

  SCM getMouse();
  SCM getMouseButtonState();

  SCM addEvent(SCM id, SCM type, SCM propname, SCM lambda);
  SCM qscm_puts(SCM str);

  SCM register_fn(SCM name, SCM fn);
  SCM get_fn(const char *name);

  SCM MsTime();
}

#endif //EDITORCONTROLLER_GUILE_H
