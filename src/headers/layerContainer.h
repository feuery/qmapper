#ifndef LAYERCONTAINER_H
#define LAYERCONTAINER_H

#include <layer.h>
#include <mapContainer.h>

class Layercontainer: public Layer{
public:
  // in tiles
  Layercontainer(int w, int h);
  Layercontainer();
  ~Layercontainer();

  virtual void set_parent(Map* p) override;
  virtual Map* parent(void) override;
  virtual int getWidth(void) override;
  virtual int getHeight(void) override;
private:

  Mapcontainer *map_parent;

};

#endif //LAYERCONTAINER_H
