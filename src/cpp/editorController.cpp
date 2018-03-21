#include <QByteArray>
#include <QBuffer>

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

    obj *tile = tc->tiles[x][y];

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

  qDebug() << "Setting tile at " << indexOfChosenLayer << ", " << x << ", " << y;
  
  m->getLayers()->at(indexOfChosenLayer)->getTiles()->at(x).at(y) = &selectedTileData;
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

void editorController::dumpTextures(ZipArchive &arch) {
  auto tilesets = document.typeRegistry<Tileset>("Tileset");
  auto sprites = document.typeRegistry<Sprite>("Sprite");
  auto animations = document.typeRegistry<animatedsprite>("AnimatedSprite");

  for(auto sprite: sprites) {
    obj* spr = sprite->getObject();
    QImage &img = spr->qi_copy;

    QTemporaryFile *f = new QTemporaryFile();
    tempFiles.push_back(f);
    
    f->open();
    if(img.save(f, "PNG")) {
      arch.addFile(sprite->getId() + ".png", f->fileName().toStdString());
      qDebug() << "Saved " << sprite->getId().c_str() << " to the zip";
    }
    else qDebug() << "Saving " << sprite->getId().c_str() << ".png failed";
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
