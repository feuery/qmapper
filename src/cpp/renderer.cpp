#include <QOpenGLFunctions>
#include <Qt>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>
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

void Renderer::mouseMoveEvent(QMouseEvent * event)
{
  auto p = event->pos();
  float x = p.x(), y = p.y();
  int w = width(), h = height();
  
  objects.first()->setLocation({(x/w) - 0.5f, -(y/h) + 0.5f, 0.0});
}

void Renderer::keyReleaseEvent(QKeyEvent *e)
{
  if(e->key() == Qt::Key_C) {
    Point p = {0.0f, 0.0f};

    makeCurrent();
    
    auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if (f) {
      objects.push_back(new immutable_obj(f,
					  "",
					  "defaultVertex.glsl",
					  "defaultFragment.glsl",
					  p));
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
  Point p = {0.0f, 0.0f};

  objects.push_back(new immutable_obj(f,
				      "",
				      "defaultVertex.glsl",
				      "defaultFragment.glsl",
				      p));
  
  
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

