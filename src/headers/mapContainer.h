#ifndef MAPCONTAINER_H
#define MAPCONTAINER_H

#include <map.h>
#include <root.h>

class Mapcontainer;
#include<layerContainer.h>

class Mapcontainer: public map{
public:
  Mapcontainer();
  Mapcontainer(int w, int h, int layerCount, root *parent);
  ~Mapcontainer();
  
  root* parent();
  void parent(root *p);

  int width() override;
  int height() override;
private:

  root *R;

};

#endif //MAPCONTAINER_H
