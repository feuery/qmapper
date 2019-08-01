#include <engine_renderer.h>
#include <editorController.h>
#include <sstream>



Engine_Renderer::Engine_Renderer(QWidget *parent): Renderer(parent)
{  
  name = "ENGINE_RENDERER";

  setMouseTracking(true);

  mouseMoveEvents.push_back([](QMouseEvent* e) {
			      editorController::instance->mouseX = e->x();
			      editorController::instance->mouseY = e->y();
			    });
  editorController::instance->engine = this;
}

void Engine_Renderer::renderEditorSpecific() {
  // no
}

void Engine_Renderer::initializeGL() {
  glClearColor(1.0f, 0.5f, 0.0f, 0.0f);
}

void Engine_Renderer::keyPressEvent(QKeyEvent *e) {
  // the old way, if someone wants to poll the state of the kbd
  editorController::instance->keyMap[static_cast<Qt::Key>(e->key())] = true;

  std::string key_str = qtkey_to_keystr(e);
  keys_down.insert(key_str);
}

void Engine_Renderer::keyReleaseEvent(QKeyEvent *e) {
  editorController::instance->keyMap[static_cast<Qt::Key>(e->key())] = false;

  std::string key_str = qtkey_to_keystr(e);
  keys_down.erase(keys_down.find(key_str));

  lisp("(setf qmapper.transitions:*message-queue* nil)");
}
