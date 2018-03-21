#include <tilesetContainer.h>
#include <editorController.h>
#include <QDebug>

// Making this return a 2d array should be a low hanging fruit when optimizing
// I'm skipping it now 'cause I have no idea how this insane language did
// dynamically sized 2D arrays
void tilesetContainer::load_texture_splitted(Renderer *parent, const char *filename)
{
  QImage root_img;
  root_img.load(QString(filename));

  tiles_w = root_img.width() / 50;
  tiles_h = root_img.height() / 50;

  tiles = new obj**[tiles_w];

  // QVector<QVector<obj*>> results(array_w);
  
  for(int x = 0; x < tiles_w; x++) {
    tiles[x] = new obj*[tiles_h];
    std::vector<Tile*> tileRow;
    
    for(int y = 0; y < tiles_h; y++) {
      QImage copy = root_img.copy(x * 50,
				  y * 50, 50, 50);
      obj *o = obj::make(parent, copy);
      o->position = glm::vec2(x * 50.0f, y * 50.0f);
      qDebug() << "Loading tiletexture at " << o;
      tiles[x][y] = o;

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

tilesetContainer::tilesetContainer(Renderer *r, const char *tilesetPath): Tileset(){
  setTiles(new std::vector<std::vector<Tile*>>);
  load_texture_splitted(r, tilesetPath);
  r->owned_objects[id] = this;
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

void tilesetContainer::render(Renderer *parent)
{
  for(int x = 0; x <  tiles_w; x++) {
    for(int y = 0; y < tiles_h; y++) {
      Renderable *tile = tiles[x][y];

      if(tile) {
  	tile->render(parent);
      }
      else qDebug() << "Tile on " << tile;
    }
  }
  
}

void tilesetContainer::render(QOpenGLFunctions_4_3_Core *f)
{
  render();
}

void tilesetContainer::render()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  for(int x = 0; x < tiles_w; x++) {
    for(int y = 0; y < tiles_h; y++) {
      obj* tile = tiles[x][y];
      if(tile) {
	tile->render(f);
      }
      else qDebug() << "Tile on " << tile;
    }
  }
}

int tilesetContainer::getRenderId()
{
  return id;
}
