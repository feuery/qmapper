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
  
protected slots:
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  float r = 0.0f;
  QTimer timer;
  
  QVector<immutable_obj*> objects;
};

#endif //RENDERER_H
