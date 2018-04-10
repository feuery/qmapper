#include <tilesetContainer.h>
#include <editorController.h>
#include <QDebug>

// Making this return a 2d array should be a low hanging fruit when optimizing
// I'm skipping it now 'cause I have no idea how this insane language did
// dynamically sized 2D arrays
void Tilesetcontainer::load_texture_splitted(Renderer *parent, const char *filename)
{
  QImage root_img;
  root_img.load(QString(filename));

  tiles_w = root_img.width() / 50;
  tiles_h = root_img.height() / 50;

  // qDebug() << "Tiles_w: " << tiles_w;

  // QVector<QVector<obj*>> results(array_w);
  // tiles = new std::string*[tiles_w];
  tiles.resize(tiles_w, std::vector<std::string>(tiles_h, ""));
  
  for(int x = 0; x < tiles_w; x++) {
    std::vector<Tile*> tileRow;
    
    for(int y = 0; y < tiles_h; y++) {
      QImage copy = root_img.copy(x * 50,
				  y * 50, 50, 50);
      obj *o = obj::make(parent, copy);
      o->position = glm::vec2(x * 50.0f, y * 50.0f);
      // qDebug() << "Loading tiletexture at " << o;
      tiles[x][y] = o->getRenderId();

      Tile *t = new Tile;
      t->setX(x);
      t->setY(y);
      t->setTileset(getId());
      tileRow.push_back(t);
    }
    try {
    getTiles()->push_back(tileRow);
    }
    catch(...) {
      puts("(Lol");
      throw;
    }
  }
}

void Tilesetcontainer::setTileSize(int tiles_w, int tiles_h)
{
  tiles.resize(tiles_w, std::vector<std::string>(tiles_h, ""));
}

Tilesetcontainer::Tilesetcontainer():Tileset() {
  setTiles(new std::vector<std::vector<Tile*>>);
}

Tilesetcontainer::Tilesetcontainer(Renderer *r, const char *tilesetPath): Tilesetcontainer(){
  load_texture_splitted(r, tilesetPath);
  r->setOwnObject(getId(), this);
  if(!editorController::instance->firstLoadedTileset) {
    for(int i = 0; i < editorController::instance->tiles->size(); i++) {
      Tile *t = editorController::instance->tiles->at(i);
      t->setTileset(getId());
    }
    
    editorController::instance->firstLoadedTileset = this;
    delete editorController::instance->tiles;
    editorController::instance->tiles = nullptr;
  }
}

void Tilesetcontainer::render(Renderer *parent)
{
  for(int x = 0; x <  tiles_w; x++) {
    for(int y = 0; y < tiles_h; y++) {
      Renderable *tile = parent->getOwnObject(tiles[x][y]);

      if(tile) {
  	tile->render(parent);
      }
      else qDebug() << "Tile on " << tile;
    }
  }
  
}

void Tilesetcontainer::render(QOpenGLFunctions_4_3_Core *f)
{
  render();
}

void Tilesetcontainer::render()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  Renderer *parent = editorController::instance->tilesetView;
  
  for(int x = 0; x < tiles.size(); x++) {
    for(int y = 0; y < tiles.at(x).size(); y++) {
      std::string renderID = tiles.at(x).at(y);
      auto result = parent->getOwnObject(renderID);
      obj* tile = static_cast<obj*>(result);;
      if(tile) {
	tile->render(f);
      }
      else qDebug() << "Tile on " << tile;
    }
  }
}

std::string Tilesetcontainer::getRenderId() const
{
  return getId();
}

std::tuple<int, int> Tilesetcontainer::getTileCoordById(std::string id)
{
  assert(getTiles() != nullptr);
  if(getTiles()->size() == 0)
    qDebug() << "getTiles()->size() => 0, things will break";
  
  for(int x = 0; x < getTiles()->size(); x++) {
    for(int y = 0; y < getTiles()->at(x).size(); y++) {
      auto t = getTiles()->at(x).at(y);
      if(t->getId() == id) return std::tuple<int, int>(x, y);
    }
  }

  return std::tuple<int, int>(-1, -1);
}
