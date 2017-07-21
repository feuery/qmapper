#include<layerContainer.h>

Layercontainer::Layercontainer(int w, int h):layer()
{
  this->w = w;
  this->h = h;
  tiles = new std::vector<std::vector<tile>>;
  for(int x = 0; x<w; x++) {
    std::vector<tile> column;
    for(int y = 0; y<h; y++) {
      column.push_back(tile());
    }
    tiles->push_back(column);
  }   
}

Layercontainer::~Layercontainer() {
  delete tiles;
}

void Layercontainer::set_parent(map* p)
{
  this->p = p;
}
map* Layercontainer::parent(void)
{
  return p;
}

int Layercontainer::getwidth(void)
{
  return w;
}
int Layercontainer::getheight(void)
{
  return h;
}
