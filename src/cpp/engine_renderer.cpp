#include <engine_renderer.h>
#include <editorController.h>

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
  editorController::instance->keyMap[static_cast<Qt::Key>(e->key())] = true;
}

void Engine_Renderer::keyReleaseEvent(QKeyEvent *e) {
  editorController::instance->keyMap[static_cast<Qt::Key>(e->key())] = false;
}
