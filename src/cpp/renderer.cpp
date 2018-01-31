#include <QOpenGLFunctions>
#include <Qt>
#include <QMouseEvent>

#include <QDebug>
#include <math.h>
#include <renderer.h>
#include <editorController.h>

Renderer::Renderer(): QOpenGLWidget()
{  

  connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
  // if(format.swapInterval() == -1) {
  //   qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
  //   timer.setInterval(17);
  // }
  // else
    timer.setInterval(0);
  timer.start();

  if(!editorController::instance->ctx_provider)
    editorController::instance->ctx_provider = this;

  // This trickery-pockery tricks the scrollarea to function as expected
  const int w = 9999;
  const int h = w;
  setMaximumWidth(w);
  setMaximumHeight(h);
  resize(w, h);
}

float distance(float x1, float y1, float x2, float y2)
{
  // TODO sqrtf might be useless here
  return abs(sqrtf(powf(x2-x1, 2.0f) + powf(y2-y1, 2.0f)));
}

Renderer::~Renderer()
{
  releaseKeyboard();
}

void Renderer::initializeGL()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>(); 
  
  if(f)
    f->glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
}

void Renderer::paintGL()
{
  editorController *ec = editorController::instance;
  if(!ec->renderingEnabled) return;
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  if(f) {
    f->glClear(GL_COLOR_BUFFER_BIT);
    
    for(auto i = drawQueue.begin(); i !=  drawQueue.end(); ++i) {
      Renderable *o = owned_objects[(*i)->getRenderId()];
      if (o) 
	o->render(this);
      else {
	qDebug () << "Couldn't find object-to-render with id " << (*i)->getRenderId();
	throw "";
      }
    }
  }
}

QOpenGLFunctions_4_3_Core* Renderer::getGlFns() {
  makeCurrent();
  return QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
}

void Renderer::freeCtx() {
  doneCurrent();
}

void Renderer::mouseMoveEvent(QMouseEvent *e) {
  for(auto fn: mouseMoveEvents) fn(e);
}

void Renderer::mousePressEvent(QMouseEvent *e) {
  for(auto fn: mouseDownEvents) fn(e);
}
void Renderer::mouseReleaseEvent(QMouseEvent *e) {
  for(auto fn: mouseUpEvents) fn(e);
}

void Renderer::resizeGL(int w, int h)
{
}

QVector<Renderable*>& Renderer::getDrawQueue()
{
  return drawQueue;
}
