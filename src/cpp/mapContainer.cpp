#include<mapContainer.h>
#include<tilesetContainer.h>
#include<editorController.h>
#include <cmath>
#include <spriteContainer.h>

double toRadians(int deg)
{
  return deg * M_PI / 180.0;
}

Mapcontainer::Mapcontainer(): Map()
{
  setLayers(new std::vector<Layer*>);
  editorController::instance->map_view->owned_objects[getId()] = this;
}

Mapcontainer::Mapcontainer(int w, int h, int layerCount, root *parent): Mapcontainer()
{
  R = parent;
  for(int i = 0; i < layerCount; i++) {
    Layercontainer *l = new Layercontainer(w, h);
    l->setName("Layer "+std::to_string(i));
    getLayers()->push_back(l);
    l->set_parent(this);
  }
}

Mapcontainer::~Mapcontainer()
{
  delete getLayers();
}

root* Mapcontainer::parent() {
  return R;
}

void Mapcontainer::parent(root *p) {
  R = p;
}


int Mapcontainer::width()
{
  return getLayers()->at(0)->getWidth();
}

int Mapcontainer::height ()
{
  return getLayers()->at(0)->getHeight();
}

std::string Mapcontainer::getRenderId() const
{
  return getId();
}

void Mapcontainer::render(Renderer *parent)
{
  auto f = parent->getGlFns();
  render(f);
  parent->freeCtx();
}

obj* tileToObj(Tile &tile, Renderer* parent)
{
  if(tile.getTileset() == "") return nullptr;
  std::string id (tile.getTileset());
  tilesetContainer *tileset = static_cast<tilesetContainer*>(editorController::instance->document.fetchRegister("Tileset", id));
  std::string tile_to_render_id = tileset->tiles[tile.getX()][tile.getY()];
  return static_cast<obj*>(parent->owned_objects[tile_to_render_id]);
}

obj* tileToObj(Tile *tile)
{
  if(tile->getTileset() == "") return nullptr;
  std::string id (tile->getTileset());
  tilesetContainer *tileset = static_cast<tilesetContainer*>(editorController::instance->document.fetchRegister("Tileset", id));
  std::string tile_to_render_id = tileset->tiles[tile->getX()][tile->getY()];
  return static_cast<obj*>(editorController::instance->map_view->owned_objects[tile_to_render_id]);
}

std::vector<Renderable*> Mapcontainer::getDrawQueue() {
  std::vector<Spritecontainer*> v = editorController::instance->document.typeRegistry<Spritecontainer>("Sprite");
  std::vector<Animatedspritecontainer*> av = editorController::instance->document.typeRegistry<Animatedspritecontainer>("AnimatedSprite");
  std::vector<Renderable*> vv;

  for(auto a: v)
    if(a->getParentmapid() == getId())
      vv.push_back(static_cast<Renderable*>(a));

  for(auto a: av)
    if(a->getParentmapid() == getId())
      vv.push_back(static_cast<Renderable*>(a));
  return vv;
}


void Mapcontainer::render(QOpenGLFunctions_4_3_Core *f)
{
  render();
}

void Mapcontainer::render()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  for(int l = 0; l < getLayers()->size(); l++) {
    for(int x = 0; x < width(); x++) {
      for(int y = 0; y < height(); y++) {
	Layer *layer = getLayers()->at(l);

	if(!layer->getVisible()) continue;
	
	Tile* tile = layer->getTiles()->at(x).at(y);

	if(tile->getTileset() != "" && layer->getOpacity() > 0) {
	  obj *tile_to_render = tileToObj(tile);
	  tile_to_render->opacity = layer->getOpacity();
	  tile_to_render->position = glm::vec2(x * 50.0f, y * 50.0f);

	  // Completely unlike the glm documentation claims, the rotation has to be in radians
	  tile_to_render->rotate = toRadians(tile->getRotation());

	  if(l > 0) {
	    Tile *subTile = getLayers()->at(l-1)->getTiles()->at(x).at(y);
	    tile_to_render->subObj = tileToObj(subTile);
	  }	  
	    
	  tile_to_render->render(f);
	}
      }
    }
  }

  // Render sprite-queue
  for(auto sprite: getDrawQueue()) {
    sprite->render(f);
  }
}



int distance(int x1, int y1,
	     int x2, int y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

either<animatedsprite*, Sprite*> Mapcontainer::findNearest(int x, int y) {
  either<animatedsprite*, Sprite*> toret;
  toret.a = nullptr;
  toret.b = nullptr;
  int minDistance = 99999;

  qDebug() << "Entering findNearest loop";

  std::vector<Renderable*> q = getDrawQueue();

  if(q.size() == 0) qDebug() << "Drawqueue is empty. We'll crash";
  else qDebug() << "DrawQueue is ok";
  
  for(Renderable* spr: q) {
    Propertierbase *b = dynamic_cast<Propertierbase*>(spr);
    std::string type = b->type_identifier();

    qDebug() << "Type is " << type.c_str();

    if(type == "Sprite") {
      Sprite *sprite = static_cast<Sprite*>(b);
      int d = distance(x, y,
		       sprite->getX(), sprite->getY());

      if(d < 0)
	qDebug() << x << ", " << y << ", " << sprite->getX() << ", " <<  sprite->getY();

      assert(d >= 0);

      if(d < minDistance) {
	qDebug() << "Found sprite where d is " << d << " and minDistance " << minDistance;
	minDistance = d;
	toret.b = sprite;
	toret.a = nullptr;
      }
      else qDebug() << d << " !< " << minDistance;
    }
    else {
      animatedsprite *a = static_cast<animatedsprite*>(b);
            int d = distance(x, y,
				  a->getX(), a->getY());

      if(d < minDistance) {
	qDebug() << "Found animation where d is " << d << " and minDistance " << minDistance;
	minDistance = d;
	toret.a = a;
	toret.b = nullptr;
      }
    }
  }

  qDebug() << "Leaving findNearest loop";

  assert(toret.a || toret.b);
  
  return toret;
}

Renderable* Mapcontainer::copyRenderable() {
  Mapcontainer *m = new Mapcontainer;
  
  for(int l = 0; l < getLayers()->size(); l++) {
    Layercontainer *layer = new Layercontainer(width(), height());
    for(int x = 0; x < width(); x++) {
      for(int y = 0; y < height(); y++) {
	
	Tile* tile = getLayers()->at(l)->getTiles()->at(x).at(y);
	layer->getTiles()->at(x).at(y) = tile;
      }
    }
    m->getLayers()->push_back(layer);
  }

  qDebug() << "Copied mapcontainer";
  return m;
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
	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  int layer_h = (*layer)->getHeight();
	  for(int i = 0; i < abs(w_diff); i++) {
	    qDebug() << "Inserting to left";
	    (*layer)->getTiles()->insert((*layer)->getTiles()->begin(),
				    (std::vector<Tile*>(layer_h, new Tile())));
	  }
	}
      }
      else if (h_anchor == RIGHT) {
	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  qDebug() << "Inserting to right";
	  int layer_h = (*layer)->getHeight();
	  for(int i = 0; i < abs(w_diff); i++) {
	    (*layer)->getTiles()->push_back(std::vector<Tile*>(layer_h, new Tile()));
	  }
	}
      }
    }
    else if (w_diff > 0) {
      if(h_anchor == LEFT) {
	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  for(int i = 0; i < w_diff; i++) {
	    (*layer)->getTiles()->erase((*layer)->getTiles()->begin());
	  }
	}
      }
      else if (h_anchor == RIGHT) {
	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  for(int i = 0; i < w_diff; i++) {
	    (*layer)->getTiles()->pop_back();
	  }
	}
      }
    }
  }

  if(old_h != h) {
    int h_diff = old_h - h;

    qDebug() << "h_diff: " << h_diff;

    if(h_diff < 0) {
      // then add
      if(v_anchor == TOP) {
	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  int layer_w = (*layer)->getWidth();
	  for(int x = 0; x < layer_w; x++) {
	    for(int i = 0; i < abs(h_diff); i++) {
	      qDebug() << "Inserting to top";
	      (*layer)->getTiles()->at(x).insert((*layer)->getTiles()->at(x).begin(),
					    new Tile());
	    }
	  }
	}
      }
      else if (v_anchor == BOTTOM) {
    	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  int layer_w = (*layer)->getWidth();
	  for(int x = 0; x < layer_w; x++) {
	    for(int i = 0; i < abs(h_diff); i++) {
	      qDebug() << "Inserting to bottom";
	      (*layer)->getTiles()->at(x).push_back(new Tile());
	    }
	  }
	}
      }
    }
    else if (h_diff > 0) {
      if(v_anchor == TOP) {
	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  int layer_w = (*layer)->getWidth();
	  for(int x = 0; x < layer_w; x++) {
	    for(int i = 0; i < abs(h_diff); i++) {
	      (*layer)->getTiles()->at(x).erase((*layer)->getTiles()->at(x).begin());
	    }
	  }
	}
      }
      else if (v_anchor == BOTTOM) {
	for(auto layer = getLayers()->begin(); layer < getLayers()->end(); layer++) {
	  int layer_w = (*layer)->getWidth();
	  for(int x = 0; x < layer_w; x++) {
	    for(int i = 0; i < abs(h_diff); i++) {
	      (*layer)->getTiles()->at(x).pop_back();
	    }
	  }
	}
      }
    }
  }
}
