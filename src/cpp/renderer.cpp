#include <QOpenGLFunctions>
#include <Qt>
#include <QMouseEvent>
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>
#include <math.h>
#include <renderer.h>

Renderer::Renderer(): QOpenGLWidget()
{
  // grabKeyboard();
  
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

