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
#include <QImage>

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

void editorController::populateMaps()
{
  cl_object make_map_with_layers = makefn("qmapper.map:make-map-with-layers"),
    pushMap = makefn("qmapper.root:push-map");
  const int w = 10, h = 10, layer_count = 4;
  
  for(int i = 0; i < 3; i++) {
    std::string name = "\""+(std::to_string(i)+"th map")+"\"";
    cl_object map = cl_funcall(5, make_map_with_layers,
			       c_string_to_object(name.c_str()),
			       ecl_make_fixnum(w), ecl_make_fixnum(h),
			       ecl_make_fixnum(layer_count));
    document.setValue( cl_funcall(3, pushMap, document.getValue(), map));
  }  
}

std::unordered_map<std::string, std::string> qimages;

obj* toObj(Renderer *r, std::string& k)
{
  return (obj*)r->getOwnObject(k);
}

std::string toKey(cl_object cl_key)
{
  cl_object format = makeSilentFn("format");
  return ecl_string_to_string(cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""), cl_key));
}

cl_object from_key(std::string &key)
{
  return c_string_to_object(std::string("\""+key+"\"").c_str());
}

cl_object load_qimage (cl_object cl_path) {
  cl_object gensym = makefn("gensym"),
    format = makeSilentFn("format"),
    cl_key = cl_funcall(1, gensym);

  std::string key = toKey(cl_key);
  
  std::string path = ecl_string_to_string(cl_path);
  QImage img;
  img.load(QString(path.c_str()));

  puts("Loaded img");

  qimages[key] = obj::make(img);

  puts("Persisted objs");
  
  return from_key(key);
}

cl_object qimg_w(cl_object kk) {
  std::string k = toKey(kk);
  obj *img = toObj(editorController::instance->w->map_view, qimages.at(k));
  int w = img->text_w;
  return ecl_make_fixnum(w);
}

cl_object qimg_h(cl_object kk) {
  std::string k = toKey(kk);
  obj *img = toObj(editorController::instance->w->map_view, qimages.at(k));
  int h = img->text_h;
  return ecl_make_fixnum(h);
}

cl_object copy_qimg(cl_object cl_src_k,
		    cl_object cl_x,
		    cl_object cl_y,
		    cl_object cl_w,
		    cl_object cl_h) {
  cl_object gensym = makefn("gensym"),
    cl_key = cl_funcall(1, gensym);

  std::string src_k = toKey(cl_src_k),
    key = toKey(cl_key);

  int x = fixint(cl_x),
    y = fixint(cl_y),
    w = fixint(cl_w),
    h = fixint(cl_h);
  
  
  QImage img = toObj(editorController::instance->map_view, qimages.at(src_k))->qi_copy;
  printf("copying from %d, %d with dimensions %d, %d\n", x, y, w, h);
  QImage copy = img.copy(x, y, w, h);
  auto copy_id = obj::make(copy);
  qimages[key] = copy_id;

  return from_key(key);
}

Renderer* editorController::getRenderer(cl_object dst_key) {
  cl_object format = makeSilentFn("FormAT");
  dst_key = cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""), dst_key);
  auto symname = makefn("symbol-name");
  std::string dest_key = ecl_string_to_string(dst_key);
  // printf("Rendering to %s\n", dest_key.c_str());
  Renderer *dst = dest_key == "MAP"? map_view:
    dest_key == "TILESET"? tilesetView:
    nullptr;
  assert(dst);
  return dst;
}

cl_object add_qimg_to_drawqueue (cl_object cl_img,
				 cl_object dest_key)
{
  Renderer *dst = editorController::instance->getRenderer(dest_key);
  if(!dst) {
    puts("Destination was invalid. Has to be one of the following: MAP, TILESET"); 
    return ECL_NIL;
  }

  std::string img = toKey(cl_img);

  obj *o = toObj(dst, qimages.at(img));
  QVector<Renderable*> vec {o};
  dst->addToDrawQueue(vec);
  
  return ECL_NIL;
}

cl_object add_lambda_to_drawqueue (cl_object dest_key,
				   cl_object lambda)
{
  Renderer *dst = editorController::instance->getRenderer(dest_key);
  dst->addToDrawQueue(lambda);
  
  return ECL_NIL;
}

cl_object clear_drawqueue(cl_object dst_key)
{
  Renderer *dst = editorController::instance->getRenderer(dst_key);
  if(!dst) {
    puts("Destination was invalid. Has to be one of the following: map, tileset"); 
    return ECL_NIL;
  } 
  dst->getDrawQueue().clear();
  return ECL_NIL;
}

cl_object clear_lispdrawqueue(cl_object dst_key)
{
  Renderer *dst = editorController::instance->getRenderer(dst_key);
  if(!dst) {
    puts("Destination was invalid. Has to be one of the following: map, tileset"); 
    return ECL_NIL;
  } 
  dst->getLispyDrawQueue().clear();
  return ECL_NIL;
}

cl_object set_y(cl_object cl_img, cl_object cl_y) {
  std::string img = toKey(cl_img);
  int y = fixint(cl_y);
  auto ec = editorController::instance;
  for(Renderer *r: ec->renderers) {
    obj *o = toObj(r, qimages.at(img));
    o->position.y = y;
  }
  return ECL_NIL;
}

cl_object set_x(cl_object cl_img, cl_object cl_x) {
  auto format = makefn("forMat");
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"cl_img is ~a~%\""), cl_img);
  assert(cl_img != ECL_NIL);
  std::string img = toKey(cl_img);
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"x is ~a~%\""), cl_x);
  int x = fixint(cl_x);
  auto ec = editorController::instance;
  
  for(Renderer *r: ec->renderers) {
    // puts("haetaan obj");
    
    obj *o = toObj(r, qimages.at(img));
    assert(o);
    // puts("sijoitetaan x ");
    o->position.x = x;
  }
  return ECL_NIL;
}

cl_object scheduleOnce(cl_object dst_key, cl_object key_lambda) {
  cl_object format = makeSilentFn("fOrmat");
  dst_key = cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""), dst_key);
  std::string dst_str = ecl_string_to_string(dst_key);
  printf("Scheduling! %s\n", dst_str.c_str());
  Renderer *dst = editorController::instance->getRenderer(dst_key);
  puts("got dst");
  assert(dst != nullptr);
  puts("dst is good");
  dst->glLambdas.enqueue([=]() {  cl_funcall(1, key_lambda ); });

  puts("Scheduled!");
  
  return ECL_NIL;
}

cl_object render(cl_object dst_key,
		 cl_object img_key) {
  // puts("In render()");
  Renderer *dst = editorController::instance->getRenderer(dst_key);
  // puts("got dst");
  std::string img = toKey(img_key);
  // puts("converted img to std::string");
  obj *o = toObj(dst, qimages.at(img));
  // puts("found o");
  o->render();
  // puts("out render");
}

editorController::editorController(): // indexOfChosenTileset(std::string("")),
   t(new Pen)
{
  puts("Looking up scheme definitions in editorController::editorController");
  lisp("(use-package :qmapper.map)");
  cl_object pushScript = makefn("qmapper.root:push-script");
  cl_object makeScript = makefn("qmapper.script:make-script");
    
  if(instance) {
    puts("There already exists an editorController");
    throw "";
  }

  instance = this;

  puts("Yritetään luoda eka skribula");
  cl_object scr = cl_funcall(5,
			     makeScript,
			     c_string_to_object("\"#version 430 core\nlayout (location = 0) in vec4 inp; // <vec2 pos, vec2 texPos>\n\n//uniform vec4 loc;\n\nout vec2 TexCoord;\n\nuniform mat4 model;\nuniform mat4 projection;\n\nvoid main()\n{\n  gl_Position = projection * model * vec4(inp.xy, 0.0, 1.0);\n  TexCoord = inp.zw;\n}\n\""),
			     c_string_to_object("\"Standard vertex shader\""),
			     c_string_to_object("\"defaultVertex\""),
			     c_string_to_object("\"glsl\""));

  puts("Luotiin eka skribula");

  auto doc = document.getValue();
  assert(doc != ECL_NIL);

  auto newDoc = cl_funcall(3, pushScript, doc, scr);
  document.setValue(newDoc);

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
  DEFUN("load-image", load_qimage, 1);
  DEFUN("image-w", qimg_w, 1);
  DEFUN("image-h", qimg_h, 1);
  DEFUN("copy-image", copy_qimg, 5);
  
  DEFUN("add-to-drawingqueue", add_qimg_to_drawqueue, 2);
  DEFUN("add-lambda-to-drawingqueue", add_lambda_to_drawqueue, 2);
  DEFUN("clear-drawingqueue", clear_drawqueue, 1);
  DEFUN("clear-lisp-drawingqueue", clear_lispdrawqueue, 1)
  DEFUN("set-img-x", set_x, 2);
  DEFUN("set-img-y", set_y, 2);
  DEFUN("do-schedule-lambda", scheduleOnce, 2);

  DEFUN("render", render, 2);

  e = new Engine(this);
  // Fucking embarrassing hack that makes opengl not die in a fire when using Engine_Renderer's ctx
  e->show();
}

editorController::~editorController()
{
}

void editorController::setSelectedTile(int x, int y, Renderer *tilesetView, tileview_renderer *tileRenderer)
{
  selectedTileX = x;
  selectedTileY = y;

  cl_object get_selected_tileset = makefn("qmapper.root:get-selected-tileset"),
    root_set_selected_tile = makefn("qmapper.root:set-root-chosenTile!"),
    format = makefn("formAt"),
    tileset = cl_funcall(2, get_selected_tileset, document.getValue());
  assert(tileset != ECL_NIL);
  cl_funcall(4, format, ECL_T, c_string_to_object("\"tileset is ~a~%\""), tileset);
  cl_object w_fn = makefn("qmapper.tileset:tileset-w"),
    h_fn = makefn("qmapper.tileset:tileset-h"),
    cl_w = cl_funcall(2, w_fn, tileset),
    cl_h = cl_funcall(2, h_fn, tileset);
  assert(cl_w != ECL_NIL);
  assert(cl_h != ECL_NIL);
  cl_object get_tile = makefn("qmapper.tileset:get-tile"),
    tile = cl_funcall(4, get_tile, tileset, ecl_make_int32_t(x), ecl_make_int32_t(y)),
    cl_key = get(tile, "gl-key");
  assert(tile != ECL_NIL);
  cl_funcall(4, format, ECL_T, c_string_to_object("\"tile is ~a\""), tile);
  auto new_doc = cl_funcall(3, root_set_selected_tile, document.getValue(), tile);
  document.setValue(new_doc);

  std::string tile_img_key = toKey(cl_key);

  int w = fixint(cl_w), h = fixint(cl_h);

  if(x >= w || y >= h) return;
  if(x < 0 || y < 0) return;

  obj* tileObj = toObj(tileRenderer, qimages.at(tile_img_key));
  puts("Found tile_surface");

  if(!tileRenderer) qDebug() << "tileRenderer is nil";
    
  tileRenderer->setSelectedTile(tileObj);

  puts("Pitäis varmaan koodata myös datan pallottelu eikä pelkkiän tekstuurien");

  // selectedTileData.setX(x);
  // selectedTileData.setY(y);
  // selectedTileData.setRotation(0);
  // selectedTileData.setTileset(indexOfChosenTileset);

  // qDebug() << "Selected tile is: {" << selectedTileData.getX() << ", " << selectedTileData.getY() << ", " << selectedTileData.getRotation() << ", " << selectedTileData.getTileset().c_str() << "}";

}

void editorController::setTileAt(int x, int y)
{
  cl_object set_chosen_tile_at = makefn("qmapper.map:set-chosen-tile-at");

  document.setValue( cl_funcall(4,
				set_chosen_tile_at,
				document.getValue(),
				ecl_make_fixnum(x),
				ecl_make_fixnum(y)));
}


void editorController::setTileRotation(int x, int y, int deg_angl) {
  cl_object set_tile_rotation = makefn("qmapper.map:set-tile-rotation-at");

  document.setValue( cl_funcall(5, set_tile_rotation,
			document.getValue(),
			ecl_make_fixnum(x),
			ecl_make_fixnum(y),
				ecl_make_fixnum(deg_angl % 360)));
}

void editorController::rotateTile90Deg(int x, int y) {
  cl_object get_chosen_tile = makefn("qmapper.root:root-chosenTile");
  cl_object set_chosen_tile = makefn("qmapper.map:set-tile-at-chosen-map");
  cl_object set_tile_rotation = makefn("qmapper.map:set-tile-rotation-at");
  cl_object get_rot = makefn("qmapper.tile:Tile-rotation");


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
  // Spritecontainer::make(w->map_view, path);
  puts("TODO implement sprites");
}

void editorController::loadAnimation(const char *path, int frameCount, int frameLifeTime) {
  // Animatedspritecontainer::make(w->map_view, path, frameCount, frameLifeTime);
  puts("TODO implement animations");
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
  return editorController::instance->document.getValue();
}

cl_object explode() {
  puts("Let's explode!");
  throw "";
  return ECL_NIL;
}

