#ifndef MAPCONTAINER_H
#define MAPCONTAINER_H

#include <map.h>
#include <root.h>

class Mapcontainer: map{
public:
  Mapcontainer();
  ~Mapcontainer();
  
  root* parent();
  void parent(root *p);
private:

  root *R;

};

#endif //MAPCONTAINER_H
