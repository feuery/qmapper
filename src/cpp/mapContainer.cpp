#include<mapContainer.h>

Mapcontainer::Mapcontainer(): map()
{
  layers = new std::vector<layer*>;
}

Mapcontainer::Mapcontainer(int w, int h, int layerCount, root *parent): Mapcontainer()
{
  R = parent;
  for(int i = 0; i < layerCount; i++) {
    Layercontainer *l = new Layercontainer(w, h);
    layers->push_back(l);
  }
}

Mapcontainer::~Mapcontainer()
{
  delete layers;
}

root* Mapcontainer::parent() {
  return R;
}

void Mapcontainer::parent(root *p) {
  R = p;
}
