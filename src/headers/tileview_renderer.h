#ifndef TILEVIEW_RENDERER_H
#define TILEVIEW_RENDERER_H

#include <renderer.h>

class Renderer;

class tileview_renderer: public Renderer {
  Q_OBJECT
public:

  tileview_renderer(QWidget *parent);
  void setSelectedTile(obj *obj);
								      
protected slots:
  virtual void paintGL() override;
  virtual void initializeGL() override;
  
private:

  obj *o;

};

#endif //TILEVIEW_RENDERER_H
