#include <QOpenGLFunctions>
#include <Qt>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>
#include <math.h>
#include <renderer.h>

Renderer::Renderer()
{
  grabKeyboard();
  
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(4,3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  setFormat(format);

  connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
  if(format.swapInterval() == -1) {
    qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
    timer.setInterval(17);
  }
  else timer.setInterval(0);
  timer.start();
}

float distance(float x1, float y1, float x2, float y2)
{
  // TODO sqrtf might be useless here
  return abs(sqrtf(powf(x2-x1, 2.0f) + powf(y2-y1, 2.0f)));
}

void Renderer::texturizeDrawingQueue(QString& texture_path)
{
  makeCurrent();
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  if(f)
    for(auto i = objects.begin(); i != objects.end(); ++i)
      (*i)->setup_texture(f, texture_path.toStdString().c_str());
  else
    qDebug() << "f is not";

  doneCurrent();
}

immutable_obj* Renderer::nearestObj(float mouse_x, float mouse_y)
{
  float shortestDistance = 9999.9f;
  immutable_obj* distance_owner = nullptr;

  for(auto i = objects.begin(); i != objects.end(); ++i) {
    Point3D pxLoc = (*i)->getPixelLocation();
    float d = distance(mouse_x, mouse_y,
		       pxLoc.x, pxLoc.y);
    if(d < shortestDistance) {
      shortestDistance = d;
      distance_owner = *i;
    }
  }

  return distance_owner;
}

void Renderer::mouseMoveEvent(QMouseEvent * event)
{
  auto p = event->pos();
  float x = p.x(), y = p.y();
  int w = width(), h = height();

  immutable_obj* nearest = nearestObj(x, y);
  if(nearest) nearest->setPixelLocation({x,y, 0.0}, w, h);
  else qDebug()<<"Finding the nearest object failed";
}

void Renderer::keyReleaseEvent(QKeyEvent *e)
{
  if(e->key() == Qt::Key_C) {
    
    makeCurrent();
    
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if (f) {
      objects.push_back(new immutable_obj(f,
					  "",
					  "defaultVertex.glsl",
					  "defaultFragment.glsl"));
    }
    doneCurrent();
  }
  else QWidget::keyReleaseEvent(e);    
}

Renderer::~Renderer()
{
  for(auto it = objects.begin(); it != objects.end(); ++it) {
    delete *it;
  }
  releaseKeyboard();
}

void Renderer::initializeGL()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  // auto o = new immutable_obj(f,
  // 				      "",
  // 				      "defaultVertex.glsl",
  // 			     "defaultFragment.glsl");
  // o->setup_texture(f, "/home/feuer/testitero.jpeg");
  // objects.push_back(o);
  
  
  if(f)
    f->glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
  else qDebug() << "f on roskaa";
}

void Renderer::paintGL()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  if(f) {
    f->glClear(GL_COLOR_BUFFER_BIT);
    
    for(auto i = objects.begin(); i != objects.end(); ++i) (*i)->render(f);
  }
  else
    qDebug() << "f on roskaa paintGL()ssä";
}

void Renderer::resizeGL(int w, int h)
{

}

