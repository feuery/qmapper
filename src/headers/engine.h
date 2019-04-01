#ifndef ENGINE_H
#define ENGINE_H

#include <editorController.h>
#include <ui_engine.h>
#include <engine_renderer.h>
#include <holder.h>

class Engine: public QMainWindow {
  Q_OBJECT
public:
  holder doc;
  
  Engine();
  Ui::Engine ui;

  Engine_Renderer *er;

private:

};

#else

class Engine;

#endif //ENGINE_H
