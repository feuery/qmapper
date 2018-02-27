#include <engine_renderer.h>
#include <editorController.h>

Engine_Renderer::Engine_Renderer(QWidget *parent): Renderer(parent)
{  
  name = "ENGINE_RENDERER";
  const int w = 9999;
  const int h = w;
  setMaximumWidth(w);
  setMaximumHeight(h);
  resize(w, h);
}

// Engine_Renderer::~Engine_Renderer()
// {
//   releaseKeyboard();
// }

void Engine_Renderer::initializeGL() {
  glClearColor(1.0f, 0.5f, 0.0f, 0.0f);
}

// void Engine_Renderer::paintGL()
// {
//   editorController *ec = editorController::instance;
//   auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
//   if(!ec->renderingEnabled) return;

//   glClear(GL_COLOR_BUFFER_BIT);
    
//   for(auto i = drawQueue.begin(); i !=  drawQueue.end(); ++i) {
//     Renderable *o = owned_objects[(*i)->getRenderId()]
//     if (o) 
//       o->render(f);
//     else {
//       qDebug () << "Couldn't find object-to-render with id " << (*i)->getRenderId();
//       throw "";
//     }
//   }

//   while(!glLambdas.empty()) {
//     auto lambda = glLambdas.dequeue();
//     lambda();
//   }
// }

// void Engine_Renderer::mouseMoveEvent(QMouseEvent *e) {
//   for(auto fn: mouseMoveEvents) fn(e);
// }

// void Engine_Renderer::mousePressEvent(QMouseEvent *e) {
//   for(auto fn: mouseDownEvents) fn(e);
// }
// void Engine_Renderer::mouseReleaseEvent(QMouseEvent *e) {
//   for(auto fn: mouseUpEvents) fn(e);
// }

// void Engine_Renderer::resizeGL(int w, int h)
// {
// }
