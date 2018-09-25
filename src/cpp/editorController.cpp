#include <guile_fn.h>
#include <QByteArray>
#include <QBuffer>
#include <QRegExp>

#include <editorController.h>

#include <new_obj.h>
#include <pen.h>

#include <QTemporaryFile>
#include <sys/param.h>

#include <editorController_guile.h>

using namespace libzippp;

editorController* editorController::instance;

void editorController::registerWindow(MainWindow *w)
{
  this->w = w;
}

cl_object make2DList(int w, int h, cl_object val) {
  return ECL_NIL;
  // SCM toret = scm_list_n(SCM_UNDEFINED);
  // for(int x = 0; x < w; x++) {
  //   SCM l = scm_list_1(val);
  //   for(int y = 1; y < h; y++) {
  //     l = scm_append(scm_list_n(l, val, SCM_UNDEFINED));
  //   }
  //   toret = scm_append(scm_list_n(toret, l, SCM_UNDEFINED));
  // }

  // return toret;
}

void editorController::populateMaps() {
  static cl_object makeMap   = makefn("make-map");
  static cl_object makeLayer = makefn("make-layer");
  static cl_object makeTile  = makefn("make-Tile");
  static cl_object setLayers = makefn("set-Map-layers!");
  static cl_object getLayers = makefn("map-layers");
  static cl_object pushMap   = makefn("push-map");
  static cl_object cons      = makefn("cons");
  
  for(int i = 0; i < 3; i++) {
    std::string name = "\""+(std::to_string(i)+"th map")+"\"";
    cl_object m = cl_funcall(4,
			     makeMap,
			     c_string_to_object(name.c_str()),
			     ECL_NIL,
			     ECL_NIL);
    for(int x = 0; x < 2; x++) {
      int w = 10 + 10 * x,
	h   = 10 + 10 * x;
      std::string lname = "\""+(std::to_string(x)+"th layer") +"\"";
      cl_object l = cl_funcall(5,
			       makeLayer,
			       c_string_to_object(lname.c_str()),
			       ecl_make_fixnum(255),
			       ECL_T,
			       make2DList(w, h, cl_funcall(5,
							   makeTile,
							   ecl_make_fixnum(0),
							   ecl_make_fixnum(0),
							   ecl_make_fixnum(0),
							   ecl_make_fixnum(0))));
      cl_object old_layers = cl_funcall(2 , getLayers, m);
      
      m = cl_funcall(3, setLayers, m, cl_funcall(3, cons, l, old_layers));
    }
    document.setValue( cl_funcall(3, pushMap, document.getValue(), m));
  }
}


editorController::editorController(): // indexOfChosenTileset(std::string("")),
   t(new Pen)
{
  puts("Looking up scheme definitions in editorController::editorController");
  lisp("(in-package :qmapper.map)");
  cl_object pushScript = makefn("push-script");
  cl_object makeScript = makefn("make-script");
    
  if(instance) {
    puts("There already exists an editorController");
    throw "";
  }

  instance = this;

  documentTreeModel = new Tilelistmodel(document);

  puts("Yritetään luoda eka skribula");
  cl_object scr = cl_funcall(5,
			     makeScript,
			     c_string_to_object("\"#version 430 core\nlayout (location = 0) in vec4 inp; // <vec2 pos, vec2 texPos>\n\n//uniform vec4 loc;\n\nout vec2 TexCoord;\n\nuniform mat4 model;\nuniform mat4 projection;\n\nvoid main()\n{\n  gl_Position = projection * model * vec4(inp.xy, 0.0, 1.0);\n  TexCoord = inp.zw;\n}\n\""),
			     c_string_to_object("\"Standard vertex shader\""),
			     c_string_to_object("\"defaultVertex\""),
			     c_string_to_object("\"glsl\""));
  puts("Luotiin eka skribula");

  document.setValue( cl_funcall(3, pushScript, document.getValue(), scr));

  puts("Tallennettiin eka skribula!");

  puts("Yritetään toista skribulaa");
  scr = cl_funcall(5, makeScript,
		   c_string_to_object("\"#version 430 core\nin vec2 TexCoord;\nout vec4 color;\n\nuniform sampler2D image;\nuniform sampler2D subTile;\nuniform int subTileBound;\nuniform vec3 spriteColor;\nuniform vec4 opacity;\n\nvoid main() {\n  vec4 texel = texture2D(image, TexCoord);\n\n  if(texel.a < 0.1) discard;\n  \n  if(subTileBound == 1) {\n    vec4 subCoord = texture2D(subTile, TexCoord);\n    color = mix(subCoord, texel, opacity.a);\n  }\n  else if (opacity.a < 1.0) {\n    color = mix(vec4(1.0, 0.0, 0.0, 1.0), texel, opacity.a);\n  }\n  else {\n    color = texel;\n  }\n}\""),
		   c_string_to_object("\"Standard fragment shader\""),
		   c_string_to_object("\"defaultFragment\""),
		   c_string_to_object("\"glsl\""));
  document.setValue( cl_funcall(3, pushScript, document.getValue(), scr));

  scr = cl_funcall(5, makeScript,
		   c_string_to_object("\"#version 430 core\nin vec2 TexCoord;\nout vec4 color;\n\nuniform sampler2D image;\nuniform sampler2D subTile;\nuniform int subTileBound;\nuniform vec3 spriteColor;\nuniform vec4 opacity;\n\nvoid main() {\n  vec4 texel = texture2D(image, TexCoord);\n\n  if(texel.a < 0.1) discard;\n  \n  if(subTileBound == 1) {\n    vec4 subCoord = texture2D(subTile, TexCoord);\n    color = mix(subCoord, texel, opacity.a);\n  }\n  else if (opacity.a < 1.0) {\n    color = mix(vec4(1.0, 0.0, 0.0, 1.0), texel, opacity.a);\n  }\n  else {\n    color = texel;\n  }\n}\""),
		   c_string_to_object("\"Standard selected tile - view's fragmentshader\""),
		   c_string_to_object("\"default.tileView\""),
		   c_string_to_object("\"glsl\""));
  document.setValue( cl_funcall(3, pushScript, document.getValue(), scr));

  DEFUN("get-current-doc", get_current_doc, 0);
  DEFUN("explode", explode, 0);

  e = new Engine(this);
  // Fucking embarrassing hack that makes opengl not die in a fire when using Engine_Renderer's ctx
  e->show();
}
	

editorController::~editorController()
{
  delete documentTreeModel;
}

void editorController::setSelectedTile(int x, int y, Renderer *tilesetView, tileview_renderer *tileRenderer)
{
  puts("TODO reimplement setSelectedTile");
  // selectedTileX = x;
  // selectedTileY = y;

  // int tilesetViewObjSize = tilesetView->getDrawQueue().size();
  // if(tilesetViewObjSize == 1) {
  //   Renderable *tileset = tilesetView->getDrawQueue().at(0);

  //   Tilesetcontainer *tc = dynamic_cast<Tilesetcontainer*>(tileset);

  //   if(x >= tc->tiles_w || y >= tc->tiles_h) return;
  //   if(x < 0 || y < 0) return;

  //   obj *tile = static_cast<obj*>(tilesetView->getOwnObject(tc->tiles[x][y]));

  //   if(!tileRenderer) qDebug() << "tileRenderer is nil";
    
  //   tileRenderer->setSelectedTile(tile);

  //   selectedTileData.setX(x);
  //   selectedTileData.setY(y);
  //   selectedTileData.setRotation(0);
  //   selectedTileData.setTileset(indexOfChosenTileset);

  //   qDebug() << "Selected tile is: {" << selectedTileData.getX() << ", " << selectedTileData.getY() << ", " << selectedTileData.getRotation() << ", " << selectedTileData.getTileset().c_str() << "}";
  // }
  // else qDebug() << "Can't update selectedtile to tileset's shader. Found " << tilesetViewObjSize << " tilesets";
}

void editorController::setTileAt(int x, int y)
{
  cl_object set_chosen_tile_at = makefn("set-chosen-tile-at");

  document.setValue( cl_funcall(4,
			set_chosen_tile_at,
			document.getValue(),
			ecl_make_fixnum(x),
				ecl_make_fixnum(y)));
}


void editorController::setTileRotation(int x, int y, int deg_angl) {
  cl_object set_tile_rotation = makefn("set-tile-rotation-at");

  document.setValue( cl_funcall(5, set_tile_rotation,
			document.getValue(),
			ecl_make_fixnum(x),
			ecl_make_fixnum(y),
				ecl_make_fixnum(deg_angl % 360)));
}

void editorController::rotateTile90Deg(int x, int y) {
  cl_object get_chosen_tile = makefn("root-chosenTile");
  cl_object set_chosen_tile = makefn("set-tile-at-chosen-map");
  cl_object set_tile_rotation = makefn("set-tile-rotation-at");
  cl_object get_rot = makefn("Tile-rotation");


  cl_object tile = cl_funcall(2,
			      get_chosen_tile,
			document.getValue());
  cl_object rotation = cl_funcall(2, get_rot, tile);
  document.setValue( cl_funcall(5, set_tile_rotation,
			document.getValue(),
			ecl_make_fixnum(x),
			ecl_make_fixnum(y),
				ecl_make_fixnum((fixint(rotation) + 90) % 360)));
  
}

void editorController::loadSprite(const char* path) {
  documentTreeModel->begin();
  // Spritecontainer::make(w->map_view, path);
  puts("TODO implement sprites");
  documentTreeModel->end();
}

void editorController::loadAnimation(const char *path, int frameCount, int frameLifeTime) {
  documentTreeModel->begin();
  // Animatedspritecontainer::make(w->map_view, path, frameCount, frameLifeTime);
  puts("TODO implement animations");
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
  puts("TODO implement saving when sprites are implemented!");
//   auto tilesets = document.typeRegistry<Tileset>("Tileset");
//   auto sprites = document.typeRegistry<Sprite>("Sprite");
//   auto animations = document.typeRegistry<animatedsprite>("AnimatedSprite");

//   for(auto sprite: sprites) {
//     obj* spr = sprite->getObject();
//     QImage &img = spr->qi_copy;
//     saveImg(arch, img, sprite->getId() + ".png");
//   }

//   for(auto animation: animations) {
//     int i = 0;
//     for(auto it = animation->sprites->begin(); it != animation->sprites->end(); it++) {
//       auto sprite = *it;
//       obj* spr = sprite->getObject();
//       QImage &img = spr->qi_copy;
//       saveImg(arch, img, animation->getId() + " - " + std::to_string(i) + ".png");
//       i++;
//     }
//   }

//   for(auto tileset: tilesets) {
//     Tilesetcontainer *c_tileset = static_cast<Tilesetcontainer*>(tileset);
//     for(int x = 0; x < c_tileset->tiles.size(); x++) {
//       for(int y = 0; y < c_tileset->tiles.at(x).size(); y++) {
// 	obj *tile_obj = static_cast<obj*>(tilesetView->getOwnObject(c_tileset->tiles.at(x).at(y)));
// 	Tile *tile = c_tileset->getTiles()->at(x).at(y);
// 	saveImg(arch, tile_obj->qi_copy, c_tileset->getId() + " - " + tile->getId() + ".png");	
//       }
//     }
//   }
}

void editorController::saveTo(QString filename) {
  
  puts("TODO implement saving when sprites are implemented!");
//   QFile f(filename);
//   f.remove();
//   ZipArchive arch(filename.toStdString());
//   arch.open(ZipArchive::WRITE);
//   qDebug() << "Trying root.toJSON();";
//   std::string rootJson = document.toJSON();

//   qDebug() << "Got root json";
  
//   arch.addData("root.json", rootJson.c_str(), rootJson.size());
//   dumpTextures(arch);
//   arch.close();
//   for(auto tmp: tempFiles) {
//     tmp->close();
//     delete tmp;
//   }
//   tempFiles.clear();
//   qDebug() << "Saved root to " << filename;
}

void editorController::loadFrom(QString fname) {
  clearDrawQueues();
  
  puts("TODO implement loading when sprites are implemented!");
  // ZipArchive arch(fname.toStdString());
  // arch.open(ZipArchive::READ_ONLY);
  // auto entries = arch.getEntries();
  // int i = 0;
  // for(ZipEntry entry: entries) {
  //   std::string name = entry.getName();
  //   if(name == "root.json") {
  //     assert( i == 0);
  //     std::string data = entry.readAsText();
  //     qDebug() << "Read root json";
  //     document.fromJSON(data.c_str());
  //   }
  //   else {
  //     assert( i != 0);
  //     QString fname = name.c_str();
  //     qDebug() << "Loading file " << fname;
  //     QRegExp sprite_regex("\\d+\\.png"),
  //   	animation_or_tileset_regex("(\\d+) - (\\d+)\\.png");

  //     if(sprite_regex.exactMatch(fname)) {
  //   	qDebug() << "Loading sprite";
  //   	QString id_str = fname.remove(".png", Qt::CaseInsensitive);
  //   	Sprite *spr = toSprite(document.fetchRegister("Sprite", id_str.toStdString()));
	
  //   	int size = entry.getSize();
  //   	void *v_data = entry.readAsBinary();
  //   	uchar *data = static_cast<uchar*>(v_data);
  //   	QImage img;
  //   	img.loadFromData(data, size);
  //   	obj::make(img, spr->getId());
  //     }
  //     else if (animation_or_tileset_regex.exactMatch(fname)) {
  //   	int pos = animation_or_tileset_regex.indexIn(fname);

  //   	if(pos < 0 ) {
  //   	  qDebug() << " pos < 0 ";
  //   	  throw "";
  //   	}
	
  //   	std::string id = animation_or_tileset_regex.cap(1).toStdString();

  //   	bool isAnimation = document.typeHasId("AnimatedSprite", id);

  //   	if(isAnimation) {
  // 	  bool isInt = false;
  // 	  int sprite_id = animation_or_tileset_regex.cap(2).toInt(&isInt);

  // 	  if(!isInt) {
  // 	    qDebug() << "Animation's frame number (" << animation_or_tileset_regex.cap(2) << ") is not a number";
  // 	    throw "";
  // 	  }

  // 	  int size = entry.getSize();
  //   	  void *v_data = entry.readAsBinary();
  //   	  uchar *data = static_cast<uchar*>(v_data);
  //   	  QImage img;
  //   	  img.loadFromData(data, size);

  // 	  Propertierbase *r = document.fetchRegister("AnimatedSprite", id);
  // 	  animatedsprite *a = r? toAnimatedsprite(r): new Animatedspritecontainer();
  // 	  Sprite *spr = new Spritecontainer;
  // 	  obj::make(img, spr->getId());
	  
  // 	  a->sprites->push_back(spr);
  //   	}
  //   	else {
  //   	  puts("Ladataan tilesettiä");
  //   	  std::string tile_id = animation_or_tileset_regex.cap(2).toStdString();	
  //   	  int size = entry.getSize();
  //   	  void *v_data = entry.readAsBinary();
  //   	  uchar *data = static_cast<uchar*>(v_data);
  //   	  QImage img;
  //   	  img.loadFromData(data, size);

  //   	  obj::make(img, tile_id);

  //   	  Tilesetcontainer *tileset = static_cast<Tilesetcontainer*>(document.fetchRegister("Tileset", id));
  //   	  auto coord = tileset->getTileCoordById(tile_id);
  //   	  int x, y;
  //   	  std::tie(x, y) = coord;

  //   	  if(x < 0 || y < 0) {
  //   	    qDebug()<< "Didn't find coords for tile " << tile_id.c_str() << " from tileset " << id.c_str();
  //   	    qDebug() << "x, y: " << x << ", " << y;
  //   	    throw "";
  //   	  }

  //   	  obj *o = static_cast<obj*>(tilesetView->getOwnObject(tile_id));
  //   	  o->position = glm::vec2(x * 50.0f, y * 50.0f);

  //   	  tileset->setTileSize(tileset->getTiles()->size(), tileset->getTiles()->at(0).size());

  //   	  printf("Loading tile to (%d, %d) with id %s\n", x, y, tile_id.c_str());

  //   	  tileset->tiles.at(x).at(y) = tile_id;
  //   	  tileset->tiles_w = MAX(tileset->tiles_w, x);
  //   	  tileset->tiles_h = MAX(tileset->tiles_h, y);
  //   	}
  //     }
  //     else {
  //   	qDebug() << "Didn't recognize fname " << fname;
  //   	throw "";
  //     }
  //   }
  //   i++;
  // }

  // for(auto it = document.getSprites()->begin(); it != document.getSprites()->end(); it++) {
  //   auto sprite = it->second;
  //   qDebug() << "sprite loading done";
  //   sprite->loadingDone();
  // }

  // for(auto it = document.getAnimatedsprites()->begin(); it != document.getAnimatedsprites()->end(); it++) {
  //   auto a = it->second;
  //   a->loadingDone();
  // }
  

  // documentTreeModel = new Tilelistmodel(&document);
  // w->setupTree();
  // loaded = true;
}

void editorController::clearDrawQueues() {
  for(auto r: renderers) {
    r->getDrawQueue().clear();
    r->clearOwnObjects();
  }
}

cl_object get_current_doc() {
  puts("Lollo, palautetaan docu");
  return editorController::instance->document.getValue();
}

cl_object explode() {
  puts("Let's explode!");
  throw "";
  return ECL_NIL;
}
