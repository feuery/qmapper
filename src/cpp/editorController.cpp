#include <QByteArray>
#include <QBuffer>
#include <QRegExp>

#include <propertierbase.h>
#include <mapContainer.h>
#include <layerContainer.h>
#include <editorController.h>
#include <spriteContainer.h>
#include <animatedspriteContainer.h>

#include <new_obj.h>
#include <script.h>
#include <pen.h>

#include <tilesetContainer.h>
#include <QTemporaryFile>
#include <sys/param.h>
using namespace libzippp;

editorController* editorController::instance;

void editorController::registerWindow(MainWindow *w)
{
  this->w = w;
}

void editorController::populateMaps() {
  for(int i = 0; i < 3; i++) {
    Mapcontainer *m = new Mapcontainer();
    m->setName(std::to_string(i)+"th map");
    m->parent(&document);
    for(int x = 0; x < 2; x++) {
      Layercontainer *l = new Layercontainer(10 + 10*x, 10 + 10*x);
      l->setName(std::to_string(x)+"th layer");
      l->set_parent(m);
      m->getLayers()->push_back(l);
    }
    (*document.getMaps())[m->getId()] = m;
  }
}

editorController::editorController(): indexOfChosenTileset(std::string("")), t(new Pen)
{

  if(instance) {
    puts("There already exists an editorController");
    throw "";
  }

  instance = this;

  documentTreeModel = new Tilelistmodel(&document);

  Script *scr = new Script;
  scr->setScript_type(glsl);
  scr->setNs("defaultVertex");
  scr->setName("Standard vertex shader");
  scr->setContents("#version 430 core\nlayout (location = 0) in vec4 inp; // <vec2 pos, vec2 texPos>\n\n//uniform vec4 loc;\n\nout vec2 TexCoord;\n\nuniform mat4 model;\nuniform mat4 projection;\n\nvoid main()\n{\n  gl_Position = projection * model * vec4(inp.xy, 0.0, 1.0);\n  TexCoord = inp.zw;\n}\n");
  (*document.getScripts())[scr->getId()] = scr;
  indexOfStdVertexShader = scr->getId();

  scr = new Script;
  scr->setScript_type(glsl);
  scr->setName("Standard fragment shader");
  scr->setNs("defaultShader");
  scr->setContents("#version 430 core\nin vec2 TexCoord;\nout vec4 color;\n\nuniform sampler2D image;\nuniform sampler2D subTile;\nuniform int subTileBound;\nuniform vec3 spriteColor;\nuniform vec4 opacity;\n\nvoid main() {\n  vec4 texel = texture2D(image, TexCoord);\n\n  if(texel.a < 0.1) discard;\n  \n  if(subTileBound == 1) {\n    vec4 subCoord = texture2D(subTile, TexCoord);\n    color = mix(subCoord, texel, opacity.a);\n  }\n  else if (opacity.a < 1.0) {\n    color = mix(vec4(1.0, 0.0, 0.0, 1.0), texel, opacity.a);\n  }\n  else {\n    color = texel;\n  }\n}");
  (*document.getScripts())[scr->getId()] = scr;
  indexOfStdFragmentShader = scr->getId();

  scr = new Script;
  scr->setScript_type(glsl);
  scr->setName("Standard selected tile - view's fragmentshader");
  scr->setNs("default.tileView");
  scr->setContents("#version 430 core\nin vec2 TexCoord;\nout vec4 color;\n\nuniform sampler2D image;\nuniform vec3 spriteColor;\nuniform vec2 selectedTileCoord; // <- in tile-coords\n\nvoid main() {\n  vec4 texel = // vec4(spriteColor, 1.0) * \n texture(image, TexCoord + (selectedTileCoord));\n if(texel.a < 0.5) discard;\n\n  color = texel;\n}");
  (*document.getScripts())[scr->getId()] = scr;
  indexOfStdTileviewFragShader = scr->getId();

  e = new Engine(this);
  // Fucking embarrassing hack that makes opengl not die in a fire when using Engine_Renderer's ctx
  e->show();
}

editorController::~editorController()
{
  delete documentTreeModel;
}

QOpenGLFunctions_4_3_Core* editorController::getGlFns()
{
  if(!ctx_provider) {
    qDebug() << "Can't get QOpenGLFunctions when provider is nil";
    throw "";
  }

  return ctx_provider->getGlFns();
}

void editorController::freeCtx()
{
  if(!ctx_provider) {
    qDebug() << "Can't get QOpenGLFunctions when provider is nil";
    throw "";
  }

  ctx_provider->freeCtx();
}

void editorController::setSelectedTile(int x, int y, Renderer *tilesetView, tileview_renderer *tileRenderer)
{
  selectedTileX = x;
  selectedTileY = y;

  int tilesetViewObjSize = tilesetView->getDrawQueue().size();
  if(tilesetViewObjSize == 1) {
    Renderable *tileset = tilesetView->getDrawQueue().at(0);

    tilesetContainer *tc = dynamic_cast<tilesetContainer*>(tileset);

    if(x >= tc->tiles_w || y >= tc->tiles_h) return;
    if(x < 0 || y < 0) return;

    obj *tile = static_cast<obj*>(tilesetView->owned_objects.at(tc->tiles[x][y]));

    if(!tileRenderer) qDebug() << "tileRenderer is nil";
    
    tileRenderer->setSelectedTile(tile);

    selectedTileData.setX(x);
    selectedTileData.setY(y);
    selectedTileData.setRotation(0);
    selectedTileData.setTileset(indexOfChosenTileset);

    qDebug() << "Selected tile is: {" << selectedTileData.getX() << ", " << selectedTileData.getY() << ", " << selectedTileData.getRotation() << ", " << selectedTileData.getTileset().c_str() << "}";
  }
  else qDebug() << "Can't update selectedtile to tileset's shader. Found " << tilesetViewObjSize << " tilesets";
}

#define check_chosen_layer   if(indexOfChosenLayer < 0) { \
  qDebug() << "IndexOfChosenLayer not valid: " << indexOfChosenLayer; \
  return; \
  }

void editorController::setTileAt(int x, int y)
{
  Map* m = toMap(document.fetchRegister("Map", indexOfChosenMap));

  check_chosen_layer
  
  m->getLayers()->at(indexOfChosenLayer)->getTiles()->at(x).at(y) = toTile(selectedTileData.copy());
}


void editorController::setTileRotation(int x, int y, int deg_angl) {
  Map *m = toMap(document.fetchRegister("Map", indexOfChosenMap));

  check_chosen_layer;

  Tile *t = m->getLayers()->at(indexOfChosenLayer)->getTiles()->at(x).at(y);
  t->setRotation(deg_angl);    
}

void editorController::rotateTile90Deg(int x, int y) {
  Map *m = toMap(document.fetchRegister("Map", indexOfChosenMap));

  check_chosen_layer;

  Tile *t = m->getLayers()->at(indexOfChosenLayer)->getTiles()->at(x).at(y);
  int new_rot = (t->getRotation() + 90);
  t->setRotation(new_rot);
}

void editorController::loadSprite(const char* path) {
  documentTreeModel->begin();
  Spritecontainer::make(w->map_view, path);
  documentTreeModel->end();
}

void editorController::loadAnimation(const char *path, int frameCount, int frameLifeTime) {
  documentTreeModel->begin();
  Animatedspritecontainer::make(w->map_view, path, frameCount, frameLifeTime);
  documentTreeModel->end();  
}

static std::vector<QTemporaryFile*> tempFiles;

void saveImg(ZipArchive &arch, QImage &img, std::string name) {
  QTemporaryFile *f = new QTemporaryFile();
  tempFiles.push_back(f);
    
  f->open();
  if(img.save(f, "PNG")) {
    arch.addFile(name, f->fileName().toStdString());
    qDebug() << "Saved " << name.c_str() << " to the zip";
  }
}

void editorController::dumpTextures(ZipArchive &arch) {
  auto tilesets = document.typeRegistry<Tileset>("Tileset");
  auto sprites = document.typeRegistry<Sprite>("Sprite");
  auto animations = document.typeRegistry<animatedsprite>("AnimatedSprite");

  for(auto sprite: sprites) {
    obj* spr = sprite->getObject();
    QImage &img = spr->qi_copy;
    saveImg(arch, img, sprite->getId() + ".png");
  }

  for(auto animation: animations) {
    int i = 0;
    for(auto it = animation->sprites->begin(); it != animation->sprites->end(); it++) {
      auto sprite = *it;
      obj* spr = sprite->getObject();
      QImage &img = spr->qi_copy;
      saveImg(arch, img, animation->getId() + " - " + std::to_string(i) + ".png");
      i++;
    }
  }

  for(auto tileset: tilesets) {
    tilesetContainer *c_tileset = static_cast<tilesetContainer*>(tileset);
    for(int x = 0; x < c_tileset->tiles.size(); x++) {
      for(int y = 0; y < c_tileset->tiles.at(x).size(); y++) {
	obj *tile_obj = static_cast<obj*>(tilesetView->owned_objects.at(c_tileset->tiles.at(x).at(y)));
	Tile *tile = c_tileset->getTiles()->at(x).at(y);
	saveImg(arch, tile_obj->qi_copy, c_tileset->getId() + " - " + tile->getId() + ".png");	
      }
    }
  }
}

void editorController::saveTo(QString filename) {
  ZipArchive arch(filename.toStdString());
  arch.open(ZipArchive::WRITE);
  qDebug() << "Trying root.toJSON();";
  std::string rootJson = document.toJSON();

  qDebug() << "Got root json";
  
  arch.addData("root.json", rootJson.c_str(), rootJson.size());
  dumpTextures(arch);
  arch.close();
  for(auto tmp: tempFiles) {
    tmp->close();
    delete tmp;
  }
  tempFiles.clear();
  qDebug() << "Saved root to " << filename;
}

void editorController::loadFrom(QString fname) {
  clearDrawQueues();
  ZipArchive arch(fname.toStdString());
  arch.open(ZipArchive::READ_ONLY);
  auto entries = arch.getEntries();
  int i = 0;
  for(ZipEntry entry: entries) {
    std::string name = entry.getName();
    if(name == "root.json") {
      assert( i == 0);
      std::string data = entry.readAsText();
      qDebug() << "Read root json";
      document.fromJSON(data.c_str());
    }
    else {
      QString fname = name.c_str();
      QRegExp sprite_regex("\\d+\\.png"),
	animation_or_tileset_regex("(\\d+) - (\\d+)\\.png");

      if(sprite_regex.exactMatch(fname)) {
	QString id_str = fname.remove(".png", Qt::CaseInsensitive);
	Sprite *spr = toSprite(document.fetchRegister("Sprite", id_str.toStdString()));
	
	int size = entry.getSize();
	void *v_data = entry.readAsBinary();
	uchar *data = static_cast<uchar*>(v_data);
	QImage img;
	img.loadFromData(data, size);
	obj::make(img, spr->getId());
      }
      else if (animation_or_tileset_regex.exactMatch(fname)) {
	int pos = animation_or_tileset_regex.indexIn(fname);

	if(pos < 0 ) {
	  qDebug() << " pos < 0 ";
	  throw "";
	}
	
	std::string id = animation_or_tileset_regex.cap(1).toStdString();	
	bool isAnimation = document.typeHasId("AnimatedSprite", id);

	if(isAnimation) {
	  qDebug() << "It's an animation. TODO write a loader for these";
	}
	else {
	  std::string tile_id = animation_or_tileset_regex.cap(2).toStdString();	
	  int size = entry.getSize();
	  void *v_data = entry.readAsBinary();
	  uchar *data = static_cast<uchar*>(v_data);
	  QImage img;
	  img.loadFromData(data, size);

	  obj::make(img, tile_id);

	  tilesetContainer *tileset = static_cast<tilesetContainer*>(document.fetchRegister("Tileset", id));
	  auto coord = tileset->getTileCoordById(tile_id);
	  int x, y;
	  std::tie(x, y) = coord;

	  if(x < 0 || y < 0) {
	    qDebug()<< "Didn't find coords for tile " << tile_id.c_str() << " from tileset " << id.c_str();
	    throw "";
	  }

	  obj *o = static_cast<obj*>(tilesetView->owned_objects.at(tile_id));
	  o->position = glm::vec2(x * 50.0f, y * 50.0f);

	  tileset->setTileSize(tileset->getTiles()->size(), tileset->getTiles()->at(0).size());

	  printf("Loading tile to (%d, %d) with id %s\n", x, y, tile_id.c_str());

	  tileset->tiles.at(x).at(y) = tile_id;
	  tileset->tiles_w = MAX(tileset->tiles_w, x);
	  tileset->tiles_h = MAX(tileset->tiles_h, y);
	}
      }
      else {
	qDebug() << "Didn't recognize fname " << fname;
	throw "";
      }
    }
    i++;
  }

  // for(auto tileset_it = document.getTilesets()->begin(); tileset_it != document.getTilesets()->end(); tileset_it++) {
  //   Tileset *t = tileset_it->second;
  //   for(int x = 0; x < t->getTiles()->size(); x++) {
  //     for(int y = 0; y < t->getTiles()->at(x).size(); y++) {
  // 	Tile *tile = t->getTiles()->at(x)[y];
  // 	tile->setX(x * 50);
  // 	tile->setY(y * 50);
  // 	tile->setTileset(t->getId());
  //     }
  //   }
  // }

  // TODO I can't run propertier now
  for(auto map_it = document.getMaps()->begin(); map_it != document.getMaps()->end(); map_it++) {
    Map *m = map_it->second;
    for(auto l_it = m->getLayers()->begin(); l_it != m->getLayers()->end(); l_it++) {
      Layer *l = *l_it;
      l->set_parent(m);
    }
  }

  documentTreeModel = new Tilelistmodel(&document);
  w->setupTree();
  loaded = true;
}

void editorController::clearDrawQueues() {
  for(auto r: renderers) {
    r->getDrawQueue().clear();
  }
}
