#include<mapContainer.h>
#include<tilesetContainer.h>
#include<editorController.h>
#include <cmath>

double toRadians(int deg)
{
  return deg * M_PI / 180.0;
}

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

obj* tileToObj(Tile &tile)
{
  if(tile.getTileset() == "") return nullptr;
  flyweight<std::string> id (tile.getTileset());
  tilesetContainer *tileset = static_cast<tilesetContainer*>(editorController::instance->document.fetchRegister("Tileset", id));
  int tile_to_render_id = tileset->tiles[tile.getX()][tile.getY()]->getRenderId();
  return static_cast<obj*>(editorController::instance->map_view->owned_objects[tile_to_render_id]);
}

void Mapcontainer::render(QOpenGLFunctions_4_3_Core *f)
{

  for(int l = 0; l < layers->size(); l++) {
    for(int x = 0; x < width(); x++) {
      for(int y = 0; y < height(); y++) {
	Layer *layer = layers->at(l);

	if(!layer->getVisible()) continue;
	
	Tile tile = layer->tiles->at(x).at(y);

	if(tile.getTileset() != "" && layer->getOpacity() > 0) {
	  obj *tile_to_render = tileToObj(tile);
	  tile_to_render->opacity = layer->getOpacity();
	  tile_to_render->position = glm::vec2(x * 50.0f, y * 50.0f);

	  // Completely unlike the glm documentation claims, the rotation has to be in radians
	  tile_to_render->rotate = toRadians(tile.getRotation());

	  if(l > 0) {
	    Tile subTile = layers->at(l-1)->tiles->at(x).at(y);
	    tile_to_render->subObj = tileToObj(subTile);
	  }	  
	    
	  tile_to_render->render(f);
	}
      }
    }
  }
}


void Mapcontainer::resize (int w,
			   int h,
			   verticalAnchor v_anchor,
			   horizontalAnchor h_anchor)
{
  int old_w = width(),
    old_h = height();
  // first horizontal, then vertical

  qDebug() << "w, oldw: " << w << ", " << old_w;

  if(old_w != w) {
    int w_diff = old_w - w;

    qDebug() << "w_diff: " << w_diff;

    if(w_diff < 0) {
      // then add
      if(h_anchor == LEFT) {
	for(auto layer = layers->begin(); layer < layers->end(); layer++) {
	  int layer_h = (*layer)->getHeight();
	  for(int i = 0; i < abs(w_diff); i++) {
	    qDebug() << "Inserting to left";
	    (*layer)->tiles->insert((*layer)->tiles->begin(),
				   (std::vector<Tile>(layer_h, Tile())));
	  }
	}
      }
      else if (h_anchor == RIGHT) {
	for(auto layer = layers->begin(); layer < layers->end(); layer++) {
	  qDebug() << "Inserting to right";
	  int layer_h = (*layer)->getHeight();
	  for(int i = 0; i < abs(w_diff); i++) {
	    (*layer)->tiles->push_back(std::vector<Tile>(layer_h, Tile()));
	  }
	}
      }
    }
    else if (w_diff > 0) {
      if(h_anchor == LEFT) {
	for(auto layer = layers->begin(); layer < layers->end(); layer++) {
	  for(int i = 0; i < w_diff; i++) {
	    (*layer)->tiles->erase((*layer)->tiles->begin());
	  }
	}
      }
      else if (h_anchor == RIGHT) {
	for(auto layer = layers->begin(); layer < layers->end(); layer++) {
	  for(int i = 0; i < w_diff; i++) {
	    (*layer)->tiles->pop_back();
	  }
	}
      }
    }
  }  
}
