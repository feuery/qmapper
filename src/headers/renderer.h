#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QTimer>
#include <QVector>
#include <QMap>
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
  QVector<std::function<void(QMouseEvent*)>> mouseDownEvents;
  QVector<std::function<void(QMouseEvent*)>> mouseUpEvents;

  virtual QVector<Renderable*>& getDrawQueue();

  QMap<int, Renderable*> owned_objects;
  std::string name;

protected:
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  virtual void mousePressEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;
							 
protected slots:
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  QVector<Renderable*> drawQueue;
  float r = 0.0f;
  QTimer timer;
};

#else

class Renderer;

#endif //RENDERER_H
