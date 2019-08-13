#ifndef mapscriptform
#define mapscriptform

#include <ui_map_scripts_ui.h>

class map_script_form: public QDialog {
  Q_OBJECT
public:
  map_script_form(const char *map_id);

  Ui_scripts_ui ui;

  void buildUI(const char *map_id);
};

#endif
