#include<mapContainer.h>

Mapcontainer::Mapcontainer(): map()
{
  layers = new std::vector<layer>;
}

Mapcontainer::~Mapcontainer()
{
  delete layers;
}
