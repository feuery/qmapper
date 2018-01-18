#include<layerContainer.h>
#include <editorController.h>

Layercontainer::Layercontainer(int w, int h):Layer()
{
  this->w = w;
  this->h = h;
  tiles = new std::vector<std::vector<Tile>>;
  for(int x = 0; x<w; x++) {
    std::vector<Tile> column;
    for(int y = 0; y<h; y++) {
      column.push_back(Tile());
    }
    tiles->push_back(column);
  }

  // editorController::instance->document.doRegister("Layer", getId(), this);
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
