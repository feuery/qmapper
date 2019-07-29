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
  // the old way, if someone wants to poll the state of the kbd
  editorController::instance->keyMap[static_cast<Qt::Key>(e->key())] = true;

  std::string key_str = qtkey_to_keystr(e);

  cl_object get_lambda = makefn("qmapper.engine_events:get-engine-lambda"),
    key_lisp_str = c_string_to_object(('"'+key_str+'"').c_str()),
    lambda = cl_funcall(2, get_lambda, key_lisp_str);

  if(lambda != ECL_NIL) {
    cl_funcall(1, lambda);
  }
  
}

void Engine_Renderer::keyReleaseEvent(QKeyEvent *e) {
  editorController::instance->keyMap[static_cast<Qt::Key>(e->key())] = false;

  lisp("(setf qmapper.transitions:*message-queue* nil)");
}
