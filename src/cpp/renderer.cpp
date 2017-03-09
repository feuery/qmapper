#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>
#include <renderer.h>

Renderer::Renderer()
{
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

void Renderer::mouseMoveEvent(QMouseEvent * event)
{
  auto p = event->pos();
  float x = p.x(), y = p.y();
  int w = width(), h = height();
  
  olio->setLocation({(x/w) - 0.5f, -(y/h) + 0.5f, 0.0});
}

Renderer::~Renderer()
{
  delete olio;
}

void Renderer::initializeGL()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  Point p = {0.0f, 0.0f};

  olio = new immutable_obj(f,
			   "",
			   "defaultVertex.glsl",
			   "defaultFragment.glsl",
			   p);
  
  
  if(f)
    f->glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
  else qDebug() << "f on roskaa";
}

void Renderer::paintGL()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  if(f) {
    f->glClear(GL_COLOR_BUFFER_BIT);
    olio->render(f);
  }
  else
    qDebug() << "f on roskaa paintGL()ssä";
}

void Renderer::resizeGL(int w, int h)
{

}

