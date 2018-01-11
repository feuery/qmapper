#include<mapContainer.h>
#include<tilesetContainer.h>
#include<editorController.h>

Mapcontainer::Mapcontainer(): Map()
{
  layers = new std::vector<Layer*>;
  id = rand();
  editorController::instance->map_view->owned_objects[id] = this;
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

int Mapcontainer::getRenderId()
{
  return id;
}

void Mapcontainer::render(Renderer *parent)
{
  auto f = parent->getGlFns();
  render(f);
  parent->freeCtx();
}

void Mapcontainer::render(QOpenGLFunctions_4_3_Core *f)
{

  for(int l = 0; l < layers->size(); l++) {
    for(int x = 0; x < width(); x++) {
      for(int y = 0; y < height(); y++) {
	Tile tile = layers->at(l)->tiles->at(x).at(y);
	

	if(tile.getTileset().get() != "") {	
	  tilesetContainer *tileset = static_cast<tilesetContainer*>(editorController::instance->document.fetchRegister("Tileset", (tile.getTileset())));
	  int tile_to_render_id = tileset->tiles[tile.getX()][tile.getY()]->getRenderId();
	  obj *tile_to_render = static_cast<obj*>(editorController::instance->map_view->owned_objects[tile_to_render_id]);
	  tile_to_render->position = glm::vec2(x * 50.0f, y * 50.0f);
	    
	  tile_to_render->render(f);
	}
      }
    }
  }
}
