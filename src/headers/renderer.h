#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QVector>
#include <obj.h>

class Renderer : public QOpenGLWidget {
  Q_OBJECT
public:
  Renderer();
  ~Renderer();

protected:
  virtual void mouseMoveEvent(QMouseEvent * event);
  virtual void keyReleaseEvent(QKeyEvent *e);						  
  
protected slots:
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  float r = 0.0f;
  QTimer timer;

  immutable_obj* nearestObj(float mouse_x, float mouse_y);
  
  QVector<immutable_obj*> objects;
};

#endif //RENDERER_H
