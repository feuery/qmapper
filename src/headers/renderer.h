#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QQueue>
#include <QTimer>
#include <QVector>
#include <QHash>
#include <new_obj.h>
#include <functional>
#include <QMouseEvent>

#include <scroll.h>
#include <unordered_map>
#include <guile_fn.h>

class Renderable;

class Renderer : public QOpenGLWidget {
  Q_OBJECT
public:
  Renderer(QWidget *parent);
  ~Renderer();

  QOpenGLFunctions_4_3_Core* getGlFns();
  void freeCtx();
  QVector<std::function<void(QMouseEvent*)>> mouseMoveEvents;
  QVector<std::function<void(QMouseEvent*)>> mouseDownEvents;
  QVector<std::function<void(QMouseEvent*)>> mouseUpEvents;

  virtual QVector<Renderable*>& getDrawQueue();
  virtual QVector<cl_object>& getLispyDrawQueue();
  
  std::string name;

  friend class Scroll;

  QQueue<std::function<void()>> glLambdas;

  void addToDrawQueue(QVector<Renderable*>&);
  void addToDrawQueue(cl_object lambda);
  void setOwnObject(std::string id, Renderable *obj);
  Renderable* getOwnObject(std::string id);
  void clearOwnObjects() {
    owned_objects.clear();
  }
protected:
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  virtual void mousePressEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;

  std::unordered_map<std::string, Renderable*> owned_objects;
							 
protected slots:
  void doRepaint();
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  QVector<Renderable*> drawQueue;
  QVector<cl_object> lispyDrawQueue;
  float r = 0.0f;
  QTimer timer;
};

#else

class Renderer;

#endif //RENDERER_H
