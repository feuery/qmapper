#ifndef NEWMAPUI
#define NEWMAPUI

#include <ui_new_map_ui.h>
#include <QDialog> 

class newMapUI: public QDialog {
  Q_OBJECT
 public:
  newMapUI();
  Ui::new_map_ui ui;
  std::function<void(int, int, int)> accept_lambda;
  std::function<void()> reject_lambda;
};

#endif
