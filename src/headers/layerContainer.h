#ifndef LAYERCONTAINER_H
#define LAYERCONTAINER_H

#include <layer.h>
#include <mapContainer.h>

class Layercontainer: public layer{
public:
  // in tiles
  Layercontainer(int w, int h);
  ~Layercontainer();

  virtual void set_parent(map* p) override;
  virtual map* parent(void) override;
  virtual int getwidth(void) override;
  virtual int getheight(void) override;
private:

  Mapcontainer *map_parent;

};

#endif //LAYERCONTAINER_H
