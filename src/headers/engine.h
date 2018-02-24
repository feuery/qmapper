#ifndef ENGINE_H
#define ENGINE_H

#include <editorController.h>
#include <ui_engine.h>
#include <engine_renderer.h>

class Engine: public QMainWindow {
  Q_OBJECT
public:

  Engine(editorController *ec);

  editorController *ec;

  Ui::Engine ui;

  Engine_Renderer *er;

private:

};

#endif //ENGINE_H
