#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <QOpenGLWidget>
#include <QQueue>
#include <QTimer>
#include <QVector>
#include <new_obj.h>
#include <QMouseEvent>


class Engine_Renderer: public QOpenGLWidget{
  Q_OBJECT
public:

  Engine_Renderer(QWidget *parent);
  ~Engine_Renderer();

  QVector<std::function<void(QMouseEvent*)>> mouseMoveEvents;
  QVector<std::function<void(QMouseEvent*)>> mouseDownEvents;
  QVector<std::function<void(QMouseEvent*)>> mouseUpEvents;

  // virtual QVector<Renderable*>& getDrawQueue();
  QVector<Renderable*> drawQueue;

  QQueue<std::function<void()>> glLambdas;

  protected:
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  virtual void mousePressEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;
							 
protected slots:
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  QTimer timer;

};

#endif //ENGINE_RENDERER_H
