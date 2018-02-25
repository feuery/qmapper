#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <QOpenGLWidget>
#include <QQueue>
#include <QTimer>
#include <QVector>
#include <new_obj.h>
#include <QMouseEvent>

#include <renderer.h>

class Engine_Renderer: public Renderer
{
  Q_OBJECT
public:

  Engine_Renderer(QWidget *parent);
  virtual void initializeGL();

};

#endif //ENGINE_RENDERER_H
