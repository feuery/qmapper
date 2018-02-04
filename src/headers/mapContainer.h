#ifndef MAPCONTAINER_H
#define MAPCONTAINER_H

#include <map.h>
#include <root.h>

#include<layerContainer.h>
#include<new_obj.h>

class Mapcontainer: public Map, public Renderable{
public:

  Mapcontainer();
  Mapcontainer(int w, int h, int layerCount, root *parent);

  ~Mapcontainer();
  
  root* parent();
  void parent(root *p);

  int width() override;
  int height() override;

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
  virtual void render(Renderer *parent) override;
  void resize (int w,
	       int h,
	       verticalAnchor v_anchor,
	       horizontalAnchor h_anchor) override;
  int id ;
  int getRenderId() override;
private:

  root *R;
  std::vector<Renderable*> getDrawQueue();
};
#else
class Mapcontainer;
#endif //MAPCONTAINER_H
