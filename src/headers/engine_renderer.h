#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <QOpenGLWidget>
#include <QQueue>
#include <QTimer>
#include <QVector>
#include <set>
#include <new_obj.h>
#include <QMouseEvent>
#include <QThread>

#include <renderer.h>

class Engine_Renderer: public Renderer
{
  Q_OBJECT
public:

  std::set<std::string> keys_down;

  Engine_Renderer(QWidget *parent);
  virtual void initializeGL();

protected:
  virtual void renderEditorSpecific() override;
  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;

};

#endif //ENGINE_RENDERER_H
