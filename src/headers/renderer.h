#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QTimer>
#include <QVector>
#include <new_obj.h>
#include <functional>
#include <QMouseEvent>

class Renderable;

class Renderer : public QOpenGLWidget {
  Q_OBJECT
public:
  Renderer();
  ~Renderer();

  QOpenGLFunctions_4_3_Core* getGlFns();
  void freeCtx();
  QVector<std::function<void(QMouseEvent*)>> mouseMoveEvents;

  virtual QVector<Renderable*>& getObjs();

protected:
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  
protected slots:
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  QVector<Renderable*> objects;
  float r = 0.0f;
  QTimer timer;
};

#endif //RENDERER_H
