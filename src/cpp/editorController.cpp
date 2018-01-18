#include <propertierbase.h>
#include <mapContainer.h>
#include <layerContainer.h>
#include <editorController.h>

#include <new_obj.h>
#include <script.h>

#include <tilesetContainer.h>


editorController* editorController::instance;

// editorController* editorController::getInstance()
// {
//   if(!instance)
//     instance = new editorController();
//   return instance;
// }

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
      m->layers->push_back(l);
    }
    flyweight<std::string> id = m->getId();
    Propertierbase *b = m;
    document.doRegister("Map", id, b);
  }
}

editorController::editorController(): indexOfChosenTileset(flyweight<std::string>(""))
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
  document.doRegister("Script", scr->getId(), scr);
  indexOfStdVertexShader = scr->getId();

  scr = new Script;
  scr->setScript_type(glsl);
  scr->setName("Standard fragment shader");
  scr->setNs("defaultShader");
  scr->setContents("#version 430 core\nin vec2 TexCoord;\nout vec4 color;\n\nuniform sampler2D image;\nuniform vec3 spriteColor;\n\nvoid main() {\n  vec4 texel = vec4(spriteColor, 1.0) * texture(image, TexCoord);\n  if(texel.a < 0.5) discard;\n\n  color = texel;\n}");
  document.doRegister("Script", scr->getId(), scr);
  indexOfStdFragmentShader = scr->getId();

  scr = new Script;
  scr->setScript_type(glsl);
  scr->setName("Standard selected tile - view's fragmentshader");
  scr->setNs("default.tileView");
  scr->setContents("#version 430 core\nin vec2 TexCoord;\nout vec4 color;\n\nuniform sampler2D image;\nuniform vec3 spriteColor;\nuniform vec2 selectedTileCoord; // <- in tile-coords\n\nvoid main() {\n  vec4 texel = // vec4(spriteColor, 1.0) * \n texture(image, TexCoord + (selectedTileCoord));\n if(texel.a < 0.5) discard;\n\n  color = texel;\n}");
  document.doRegister("Script", scr->getId(), scr);
  indexOfStdTileviewFragShader = scr->getId();
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

void editorController::setTileAt(int x, int y)
{
  Map* m = toMap(document.fetchRegister("Map", indexOfChosenMap));
  if(indexOfChosenLayer < 0) {
    qDebug() << "IndexOfChosenLayer is small " << indexOfChosenLayer;
    return;
  }

  qDebug() << "Setting tile at " << indexOfChosenLayer << ", " << x << ", " << y;
  
  m->layers->at(indexOfChosenLayer)->tiles->at(x).at(y) = selectedTileData;
}
