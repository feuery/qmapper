#ifndef EDITORCONTROLLER_GUILE_H
#define EDITORCONTROLLER_GUILE_H

#include <editorController.h>
/* #include <guile_qt_keymapping.h> */

extern "C" {
  cl_object add_map(cl_object s_w, cl_object s_h, cl_object s_layerCount);
  cl_object add_layer(cl_object map_index);
  cl_object delete_layer(cl_object map_index, cl_object layer_index);
  cl_object add_glsl_script();
  cl_object add_scheme_script();
  cl_object resize_current_map(cl_object w, cl_object h, cl_object horizontal_anchor, cl_object vertical_anchor);
  cl_object toggle_rendering();

  cl_object load_sprite(cl_object s_path);
  cl_object load_animation(cl_object s_path, cl_object s_framecount, cl_object s_frameLifeTime);

  cl_object keyDown(cl_object key);

  cl_object getMouse();
  cl_object getMouseButtonState();

  cl_object addEvent(cl_object id, cl_object type, cl_object propname, cl_object lambda);
  cl_object qscm_puts(cl_object str);

  cl_object register_fn(cl_object name, cl_object fn);
  cl_object get_fn(const char *name);

  cl_object MsTime();
}

#endif //EDITORCONTROLLER_GUILE_H
