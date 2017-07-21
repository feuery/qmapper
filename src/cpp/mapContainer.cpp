#include<mapContainer.h>

Mapcontainer::Mapcontainer(): map()
{
  layers = new std::vector<layer*>;
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
