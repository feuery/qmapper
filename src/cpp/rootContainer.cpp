#include <tilelistmodel.h>
#include<rootContainer.h>

Rootcontainer::Rootcontainer(): root()
{
  all_maps = new std::vector<map*>;
}

Rootcontainer::~Rootcontainer()
{
  delete all_maps;
}
