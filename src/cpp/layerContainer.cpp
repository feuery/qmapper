#include<layerContainer.h>
#include <editorController.h>

Layercontainer::Layercontainer(int w, int h):Layer()
{
  this->w = w;
  this->h = h;
  tiles = new std::vector<std::vector<Tile>>(w, std::vector<Tile>(h, Tile()));
  if(editorController::instance->tiles)
    for(int x = 0; x < tiles->size(); x++) {
      for(int y = 0; y < tiles->at(x).size(); y ++)
	editorController::instance->tiles->push_back(&tiles->at(x)[y]);
    }
}

Layercontainer::~Layercontainer() {
  delete tiles;
}

void Layercontainer::set_parent(Map* p)
{
  this->p = p;
}
Map* Layercontainer::parent(void)
{
  return p;
}

int Layercontainer::getWidth(void)
{
  return w;
}
int Layercontainer::getHeight(void)
{
  return h;
}
