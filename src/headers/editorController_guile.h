#ifndef EDITORCONTROLLER_GUILE_H
#define EDITORCONTROLLER_GUILE_H

#include <libguile.h>
#include <editorController.h>
#include <mapContainer.h>

extern "C" {
  SCM add_map(SCM s_w, SCM s_h, SCM s_layerCount);
  SCM add_layer(SCM map_index);
}

#endif //EDITORCONTROLLER_GUILE_H