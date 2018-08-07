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

class Renderable;

class Renderer : public QOpenGLWidget {
  Q_OBJECT
public:
  Renderer(QWidget *parent);
  ~Renderer();

  QVector<std::function<void(QMouseEvent*)>> mouseMoveEvents;
  QVector<std::function<void(QMouseEvent*)>> mouseDownEvents;
  QVector<std::function<void(QMouseEvent*)>> mouseUpEvents;

  virtual QVector<Renderable*>& getDrawQueue();
  
  std::string name;

  friend class Scroll;

  QQueue<std::function<void()>> glLambdas;

  void addToDrawQueue(QVector<Renderable*>&);
  void setOwnObject(std::string id, Renderable *obj);
  Renderable* getOwnObject(std::string id);
  void clearOwnObjects() {
    owned_objects.clear();
  }
protected:
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  virtual void mousePressEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;
							 
protected slots:
  void doRepaint();
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  QVector<Renderable*> drawQueue;
  float r = 0.0f;
  QTimer timer;
  std::unordered_map<std::string, Renderable*> owned_objects;
};

#else

class Renderer;

#endif //RENDERER_H
