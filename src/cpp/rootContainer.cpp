#include <tilelistmodel.h>
#include<rootContainer.h>

Rootcontainer::Rootcontainer(): root()
{
  // TODO replace with a vector<unique_pointer<map*>> or smthng which doesn't leak like a sieve
  all_maps = new std::vector<Map*>;
}

Rootcontainer::~Rootcontainer()
{
  delete all_maps;
}
