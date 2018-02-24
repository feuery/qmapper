#include <engine_renderer.h>
#include <editorController.h>

Engine_Renderer::Engine_Renderer(QWidget *parent): QOpenGLWidget(parent)
{  

  connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
  timer.setInterval(0);
  timer.start();

  // This trickery-pockery tricks the scrollarea to function as expected
  const int w = 9999;
  const int h = w;
  setMaximumWidth(w);
  setMaximumHeight(h);
  resize(w, h);

}

Engine_Renderer::~Engine_Renderer()
{
  releaseKeyboard();
}

void Engine_Renderer::initializeGL() {
  glClearColor(1.0f, 0.5f, 0.0f, 0.0f);
}

void Engine_Renderer::paintGL()
{
  editorController *ec = editorController::instance;
  if(!ec->renderingEnabled) return;

  glClear(GL_COLOR_BUFFER_BIT);
    
  for(auto i = drawQueue.begin(); i !=  drawQueue.end(); ++i) {
    Renderable *o = *i ; // owned_objects[(*i)->getRenderId()]
    if (o) 
      o->render();
    else {
      qDebug () << "Couldn't find object-to-render with id " << (*i)->getRenderId();
      throw "";
    }
  }

  while(!glLambdas.empty()) {
    auto lambda = glLambdas.dequeue();
    lambda();
  }
}

void Engine_Renderer::mouseMoveEvent(QMouseEvent *e) {
  for(auto fn: mouseMoveEvents) fn(e);
}

void Engine_Renderer::mousePressEvent(QMouseEvent *e) {
  for(auto fn: mouseDownEvents) fn(e);
}
void Engine_Renderer::mouseReleaseEvent(QMouseEvent *e) {
  for(auto fn: mouseUpEvents) fn(e);
}

void Engine_Renderer::resizeGL(int w, int h)
{
}
