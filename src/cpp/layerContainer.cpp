#include<layerContainer.h>
#include <editorController.h>

Layercontainer::Layercontainer(int w, int h):Layer()
{
  this->w = w;
  this->h = h;  
  setTiles(new std::vector<std::vector<Tile*>>(w, std::vector<Tile*>(h, new Tile())));
  if(editorController::instance->tiles)
    for(int x = 0; x < getTiles()->size(); x++) {
      for(int y = 0; y < getTiles()->at(x).size(); y ++)
	editorController::instance->tiles->push_back(getTiles()->at(x)[y]);
    }
}

Layercontainer::Layercontainer():Layer()
{
  w = h = 0;
  // For JSON mapping purposes, 
  setTiles(new std::vector<std::vector<Tile*>>(w, std::vector<Tile*>(h, new Tile())));
}

Layercontainer::~Layercontainer() {
  delete getTiles();
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
  return getTiles()->size();
}
int Layercontainer::getHeight(void)
{
  return getTiles()->at(0).size();
}
