// #define LOG_RENDER_LAMBDAS

#include <QOpenGLFunctions>
#include <Qt>
#include <QMouseEvent>

#include <QDebug>
#include <math.h>
#include <renderer.h>
#include <editorController.h>

void Renderer::doRepaint() {
  repaint();
}  

Renderer::Renderer(QWidget *parent): QOpenGLWidget(parent)
{  

  connect(&timer, &QTimer::timeout, this, &Renderer::doRepaint);
  // if(format.swapInterval() == -1) {
  //   qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
  //   timer.setInterval(17);
  // }
  // else
  timer.setInterval(0);
  timer.start();

  // This trickery-pockery tricks the scrollarea to function as expected
  const int w = 2000;
  const int h = w;
  setMaximumWidth(w);
  setMaximumHeight(h);
  resize(w, h);

  editorController::instance->renderers.push_back(this);
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
  if(!f) return;
  f->glClear(GL_COLOR_BUFFER_BIT);
    
  for(auto i = drawQueue.begin(); i !=  drawQueue.end(); ++i) {
    auto id = (*i)->getRenderId();
    Renderable *o = owned_objects[id];
    o->render(this);
  }
#ifdef LOG_RENDER_LAMBDAS
  cl_object format = makefn("format");
  for(cl_object o: getLispyDrawQueue()) {
      cl_funcall(4, format, ECL_T, c_string_to_object("\"o is ~a~%\""), o);
      cl_funcall(1, o);
    }
#else
    for(cl_object o: getLispyDrawQueue()) {
      cl_funcall(1, o);
    }
#endif

  while(!glLambdas.empty()) {
    auto lambda = glLambdas.dequeue();
    lambda();
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

QVector<cl_object>& Renderer::getLispyDrawQueue()
{
  return lispyDrawQueue;
}

void Renderer::addToDrawQueue(QVector<Renderable*>& v)
{
  drawQueue << v;
}

void Renderer::addToDrawQueue(cl_object lambda)
{
  lispyDrawQueue << lambda;
}

void Renderer::setOwnObject(std::string id, Renderable *obj) {
  // printf("registered object %s in %s\n", id.c_str(), name.c_str());
  owned_objects[id] = obj;
  printf("sizeof %s's owned_objects: %d\n", name.c_str(), owned_objects.size());
  assert(owned_objects.size() > 0);
}

Renderable* Renderer::getOwnObject(std::string id) {  
  auto it = owned_objects.find(id);

  if(it == owned_objects.end()) {
    printf("can't find %s in %s\n", id.c_str(), name.c_str());
    // printf("owned_object's size in %s is %d\n", name.c_str(), owned_objects.size());
    return nullptr;
  }

  return it->second;

  // assert(owned_objects.size() > 0);

  // return owned_objects.at(id);
}
