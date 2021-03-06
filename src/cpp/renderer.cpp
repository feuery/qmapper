// #define LOG_RENDER_LAMBDAS

#include <QOpenGLFunctions>
#include <Qt>
#include <QMouseEvent>

#include <QDebug>
#include <math.h>
#include <renderer.h>
#include <editorController.h>

#include <colored_rect.h>
#include <guile_qt_keymapping.h>

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

std::string qtkey_to_keystr(QKeyEvent *e) {
  Qt::Key key = static_cast<Qt::Key>(e->key());
  return std::string(e->modifiers() & Qt::ControlModifier? "C-": "") +
    std::string(e->modifiers() & Qt::ShiftModifier? "S-":"") + 
    qt_cl_key_pairs.at(key);
}

void Renderer::keyPressEvent(QKeyEvent *e) {
  std::string key_str = qtkey_to_keystr(e);

  cl_object key_lisp_str = c_string_to_object(('"'+key_str+'"').c_str()),
    format = makefn("format"),
    get_key_lambda = makefn("qmapper.editor_events:get-key-lambda");
  
  printf("Pressed dynamically found %s\n", key_str.c_str());
  cl_funcall(4, format, ECL_T, c_string_to_object("\"Pressed ~a in the lisp world~%\""), key_lisp_str);

  cl_object final_event = cl_funcall(2, get_key_lambda, key_lisp_str);
  if(final_event != ECL_NIL) {
    puts("Found a lisp event");
    cl_funcall(1, final_event);
  }
  else puts("Didn't find a lisp event");
}

void Renderer::initializeGL()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  
  if(f) {
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
  }
}

int getComponent(cl_object nth, cl_object selection, int ind) {
  return ecl_to_int(cl_funcall(3, nth, selection, ecl_make_int32_t(ind))) * 50;
}

void Renderer::renderEditorSpecific() {
  if(this == editorController::instance->map_view) {
    auto ec = editorController::instance;
    cl_object doc = ec->document.getValue(),
      selection = get(doc, "SELECTED-COORDINATES"),
      nth = makeSilentFn("fset:lookup"),
      format = makeSilentFn("format");
    int left = getComponent(nth, selection, 0),
      top = getComponent(nth, selection, 1),
      right = getComponent(nth, selection, 2),
      bottom = getComponent(nth, selection, 3),
      w = right - left,
      h = bottom - top;

    if(w > 0 && h > 0) {
      // printf("left %d, top %d, right %d, bottom %d, w %d, h %d\n", left, top, right, bottom, w, h);
      // printf("Piirretään kohtaan %d, %d\n", left, top);
      // cl_funcall(5, format, ECL_T, c_string_to_object("\"Piirretään kohtaan ~a, ~a\""), ecl_make_int32_t(left * 50), ecl_make_int32_t(top * 50));
      QColor c(0, 0, 255, 127);
      colored_rect rect(this, w, h, c);
      rect.position.x = left ;
      rect.position.y = top;
      rect.opacity = 127;
      rect.render();
    }
  }
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

  cl_object renderer_name = c_string_to_object((std::string("\"")+name+"\"").c_str());
  
#ifdef LOG_RENDER_LAMBDAS
  cl_object format = makefn("format");
  for(cl_object o: getLispyDrawQueue()) {
      cl_funcall(4, format, ECL_T, c_string_to_object("\"o is ~a~%\""), o);
      cl_funcall(2, o, renderer_name );
    }
#else
    for(cl_object o: getLispyDrawQueue()) {
      cl_funcall(2, o, renderer_name);
    }
#endif

  while(!glLambdas.empty()) {
    auto lambda = glLambdas.dequeue();
    lambda();
  }

  renderEditorSpecific();
}

void Renderer::deleteOwnObject(std::string &id) {
  owned_objects.erase(id);
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
