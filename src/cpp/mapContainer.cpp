#include<mapContainer.h>

Mapcontainer::Mapcontainer(): Map()
{
  layers = new std::vector<Layer*>;
}

Mapcontainer::Mapcontainer(int w, int h, int layerCount, root *parent): Mapcontainer()
{
  R = parent;
  for(int i = 0; i < layerCount; i++) {
    Layercontainer *l = new Layercontainer(w, h);
    l->setName("Layer "+std::to_string(i));
    layers->push_back(l);
    l->set_parent(this);
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


int Mapcontainer::width()
{
  return layers->at(0)->getWidth();
}

int Mapcontainer::height ()
{
  return layers->at(0)->getHeight();
}
