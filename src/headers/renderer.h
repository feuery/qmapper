#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QVector>
#include <obj.h>

#include <editorController.h>

class Renderer : public QOpenGLWidget {
  Q_OBJECT
public:
  Renderer();
  ~Renderer();

  GLuint load_texture(const char *path, int *w, int *h);

  QOpenGLFunctions_4_3_Core* getGlFns();
  void freeCtx();

  QVector<immutable_obj*> objects;
  
protected slots:
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
  virtual void initializeGL();

private:
  float r = 0.0f;
  QTimer timer;
};

#endif //RENDERER_H
