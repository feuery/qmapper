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
#include <guile_qt_keymapping.h>
#include <QImage>
#include <sstream>
#include <iterator>

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
    pushMap = makefn("qmapper.root:push-map"),
    format = makefn("format");
  const int w = 10, h = 10, layer_count = 4;
  
  for(int i = 0; i < 3; i++) {
    std::string name = "\""+(std::to_string(i)+"th map")+"\"";
    document.setValue(cl_funcall(6, make_map_with_layers,
				 document.getValue(),
				 c_string_to_object(name.c_str()),
				 ecl_make_fixnum(w), ecl_make_fixnum(h),
				 ecl_make_fixnum(layer_count)));

    // document.setValue( cl_funcall(3, pushMap, document.getValue(), map));
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

cl_object qimg_file_dimensions(cl_object cpath) {
  std::string path = ecl_string_to_string(cpath);
  QImage img;
  img.load(QString(path.c_str()));
  return cl_list(2, ecl_make_fixnum(img.width()), ecl_make_fixnum(img.height()));
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
  std::string dest_key = ecl_string_to_string(dst_key);
  // printf("Rendering to %s\n", dest_key.c_str());
  Renderer *dst = dest_key == "MAP"? map_view:
    dest_key == "TILESET"? tilesetView:
    dest_key == "ENGINE"? engine:
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

int real_fixint(cl_object integer) {
  auto floor = makeSilentFn("floor");
  fixint(cl_funcall(2, floor, integer));
}

cl_object add_lambda_to_drawqueue (cl_object dest_key,
				   cl_object lambda)
{
  Renderer *dst = editorController::instance->getRenderer(dest_key);
  // printf("dst %s engine\n", (dst == editorController::instance->engine? "is": "isn't"));
  auto format = makefn("format");
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

cl_object set_y_dst(cl_object dst_key, cl_object cl_img, cl_object cl_y) {
  std::string img = toKey(cl_img);
  int y = real_fixint(cl_y);
  auto ec = editorController::instance;
  Renderer *r = editorController::instance->getRenderer(dst_key);
  obj *o = toObj(r, qimages.at(img));
  o->position.y = y;
  
  return ECL_NIL;
}

std::vector<std::string> map_keys(std::unordered_map<std::string, std::string>& map) {
  std::vector<std::string> res;
  for(auto it: map)
    res.push_back(it.first);
  return res;
}

std::string join(std::vector<std::string>& strings) {
  const char* const delim = ", ";

  std::ostringstream imploded;
  std::copy(strings.begin(), strings.end(),
	    std::ostream_iterator<std::string>(imploded, delim));
  return imploded.str();
}

cl_object set_x_dst(cl_object dst_key, cl_object cl_img, cl_object cl_x) {
  auto format = makefn("forMat");
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"cl_img is ~a~%\""), cl_img);
  assert(cl_img != ECL_NIL);
  std::string img = toKey(cl_img);
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"x is ~a~%\""), cl_x);
  int x = real_fixint(cl_x);
  auto ec = editorController::instance;
  
  Renderer *r = editorController::instance->getRenderer(dst_key);
  auto keys = map_keys(qimages);
  std::string keys_str = join(keys);

  try {
    std::string final_key = qimages.at(img);
    obj *o = toObj(r, final_key);
    o->position.x = x;
    return ECL_NIL;
  }
  catch(std::out_of_range ex) {
    printf("out_of_range, key: %s, keys %s\n", img.c_str(), keys_str.c_str());
    throw ex;
  }
}

cl_object set_opacity_dst(cl_object dst_key, cl_object cl_img, cl_object opacity) {
  assert(cl_img != ECL_NIL);
  std::string img = toKey(cl_img);
  // cl_funcall(5, format, ECL_T, c_string_to_object("\"opacity is ~a (~a)~%\""), opacity, cl_funcall(2, type_of, opacity));
  
  int op = real_fixint(opacity);
  auto ec = editorController::instance;
  
  Renderer *r = editorController::instance->getRenderer(dst_key);    
  obj *o = toObj(r, qimages.at(img));
  o->opacity = op % 256;
  return ECL_NIL;
}

cl_object set_img_rotation(cl_object dst_key, cl_object cl_img, cl_object angle) {
  std::string img = toKey(cl_img);
  auto format = makeSilentFn("format"),
    angle_str = cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""), angle);
  std::string angle_std_str = ecl_string_to_string(angle_str);
  
  double rotation = atof(angle_std_str.c_str());
;
  auto ec = editorController::instance;
  
  Renderer *r = editorController::instance->getRenderer(dst_key);    
  obj *o = toObj(r, qimages.at(img));
  o->rotate = rotation;
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
  if(img_key == ECL_NIL) {
    puts("img_key is ECL_NIL. Why are you trying to render that?");
    return img_key;
  }
  // puts("In render()");
  Renderer *dst = editorController::instance->getRenderer(dst_key);
  // puts("got dst");
  std::string img = toKey(img_key);
  // puts("converted img to std::string");

  obj *o = toObj(dst, qimages.at(img));
  o->render();
  return ECL_NIL;
}

#define notnil(x) assert(x != ECL_NIL)
cl_object set_subobj(cl_object dst_key,
		     cl_object tile_key,
		     cl_object subobj_key) {
  notnil(dst_key);
  notnil(tile_key);
  notnil(subobj_key);

  Renderer *dst = editorController::instance->getRenderer(dst_key);
  std::string stdtile = toKey(tile_key),
    stdsubobj = toKey(subobj_key);

  obj *tile = toObj(dst, qimages.at(stdtile)),
    *sub = toObj(dst, qimages.at(stdsubobj));

  tile->subObj = sub;
}
  

cl_object MsTime() {
    return ecl_make_long(time(nullptr) * 1000);
}

cl_object keyDown(cl_object key) {
  Qt::Key k = cl_qt_key_pairs.at(ecl_string_to_string(key));
  return editorController::instance->keyMap[k]? ECL_T: ECL_NIL;
}

editorController::editorController(): // indexOfChosenTileset(std::string("")),
   t(new Pen)
{
  puts("Looking up lisp definitions in editorController::editorController");
  lisp("(use-package :qmapper.map)");
  cl_object pushScript = makefn("qmapper.root:push-script");
  cl_object makeScript = makefn("qmapper.script:make-script");

  DEFUN("keyDown", keyDown, 1)
  
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
  DEFUN("set-img-x", set_x_dst, 3);
  DEFUN("set-img-opacity", set_opacity_dst, 3)
  DEFUN("set-img-y", set_y_dst, 3);
  DEFUN("set-img-rotation", set_img_rotation, 3);
  DEFUN("set-img-subobj", set_subobj, 3);
  DEFUN("do-schedule-lambda", scheduleOnce, 2);

  DEFUN("render", render, 2);
  DEFUN("MsTime", MsTime, 0);
  DEFUN("image-file-dimensions", qimg_file_dimensions, 1);
    
  if(instance) {
    puts("There already exists an editorController");
    throw "";
  }

  instance = this;

  cl_object scr = cl_funcall(5,
			     makeScript,
			     c_string_to_object("\"#version 430 core\nlayout (location = 0) in vec4 inp; // <vec2 pos, vec2 texPos>\n\n//uniform vec4 loc;\n\nout vec2 TexCoord;\n\nuniform mat4 model;\nuniform mat4 projection;\n\nvoid main()\n{\n  gl_Position = projection * model * vec4(inp.xy, 0.0, 1.0);\n  TexCoord = inp.zw;\n}\n\""),
			     c_string_to_object("\"Standard vertex shader\""),
			     c_string_to_object("\"defaultVertex\""),
			     c_string_to_object("\"glsl\""));

  auto doc = document.getValue();
  assert(doc != ECL_NIL);

  auto newDoc = cl_funcall(3, pushScript, doc, scr);
  document.setValue(newDoc);

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
  
  e = new Engine;
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

  document.setValue(cl_funcall(5, set_tile_rotation,
			       document.getValue(),
			       ecl_make_fixnum(x),
			       ecl_make_fixnum(y),
			       ecl_make_fixnum(deg_angl % 360)));
}

void editorController::rotateTile90Deg(int x, int y) {
  cl_object get_chosen_tile = makefn("qmapper.map:get-tile-at2"),
    root_layer = makefn("qmapper.root:root-chosenLayerInd");
  cl_object set_chosen_tile = makefn("qmapper.map:set-tile-at-chosen-map");
  cl_object set_tile_rotation = makefn("qmapper.map:set-tile-rotation-at");
  cl_object get_rot = makefn("qmapper.tile:Tile-rotation");

  puts("fetching tile");
  cl_object tile = cl_funcall(4,
			      get_chosen_tile,
			      cl_funcall(2, root_layer, document.getValue()),
			      ecl_make_fixnum(x),
			      ecl_make_fixnum(y));
  puts("got tile");

  assert(tile != ECL_NIL);
  
  cl_object rotation = cl_funcall(2, get_rot, tile);
  puts("got rotation");
  document.setValue( cl_funcall(5, set_tile_rotation,
			document.getValue(),
			ecl_make_fixnum(x),
			ecl_make_fixnum(y),
				ecl_make_fixnum((fixint(rotation) + 90) % 360)));
  puts("set rotation");
  
}

void editorController::loadSprite(const char* path) {
  // Spritecontainer::make(w->map_view, path);
  cl_object load_sprite = makefn("qmapper.sprite:load-sprite");
  QString qpath = path;
  qpath = "\"" + qpath + "\"";
  cl_object cl_path = c_string_to_object(qpath.toStdString().c_str());
  document.setValue(cl_funcall(3, load_sprite, document.getValue(), cl_path));
  puts("We're back in c++ land!");
}

void editorController::loadAnimation(const char *path, int frameCount, int frameLifeTime) {
  // Animatedspritecontainer::make(w->map_view, path, frameCount, frameLifeTime);

  cl_object load_animation = makefn("qmapper.animatedsprite:load-animation"),
    push_animation = makefn("qmapper.root:push-animation"),
    animation = cl_funcall(4, load_animation, c_string_to_object((std::string("\"")+path+"\"").c_str()),
			   ecl_make_fixnum(frameCount), ecl_make_fixnum(frameLifeTime));
  

  document.setValue(cl_funcall(4, push_animation, document.getValue(), get(getChosenMap(), "ID"), animation));
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
  cl_object get_tilesets = makefn("qmapper.root:root-tilesets"),
    get_sprites = makefn("qmapper.root:root-sprites"),
    clean_key = makefn("qmapper.std:clean-key"),
    get_animations = makefn("qmapper.root:root-animatedSprites"),
    concat = lisp("(lambda (l) (let* ((l (fset:convert 'list l))) (apply #'fset:concat l)))"),
    get_keys = lisp("(lambda (l) (mapcar (lambda (e) (qmapper.std:get-prop e \"gl-key\")) (fset:convert 'list l)))"),
    toList = makefn("qmapper.std:to-list"),
    len = makefn("fset:size"),
    get_prop = makefn("qmapper.std:get-prop"),
    format = makefn("format"),
    cdr = makefn("cdr"),
    car = makefn("car"),
    tilesets = cl_funcall(2, toList, cl_funcall(2, get_tilesets, document.getValue())),
    get_tiles = makefn("qmapper.tileset:tileset-tiles");

  cl_funcall(4, format, ECL_T, c_string_to_object("\"tilesets are ~a~%\""), tilesets);
  int tileset_len = fixint(cl_funcall(2, len, tilesets));
  
  for(int i = 0; i < tileset_len; i++) {
    cl_object tileset_pair = cl_funcall(3, get_prop, tilesets, ecl_make_int32_t(i)),
      tileset_id = cl_funcall(2, car, tileset_pair),
      tileset = cl_funcall(2, cdr, tileset_pair),
      tiles1 =  cl_funcall(2, get_tiles, tileset),
      tiles = cl_funcall(2, concat, tiles1),
      gl_keys = cl_funcall(2, get_keys, tiles),
      gl_keys_len = cl_funcall(2, len, gl_keys);

    for(int gl_key_i = 0; gl_key_i < fixint(gl_keys_len); gl_key_i++) {
      cl_object gl_key = cl_funcall(3, get_prop, gl_keys, ecl_make_int32_t(gl_key_i));
      std::string key = toKey(gl_key);
      obj *o = toObj(tilesetView, qimages.at(key));
      
      QImage &img = o->qi_copy;
      saveImg(arch, img, key+".png");
    }
  }
    
  cl_object sprites = cl_funcall(2, toList, cl_funcall(2, get_sprites, document.getValue())),
    sprites_len = cl_funcall(2, len, sprites);
  for(int i = 0; i < fixint(sprites_len); i++) {
    cl_object sprite_pair = cl_funcall(3, get_prop, sprites, ecl_make_int32_t(i)),
      sprite_id = cl_funcall(2, clean_key, cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""), cl_funcall(2, car, sprite_pair))),
      sprite = cl_funcall(2, cdr, sprite_pair),
      gl_key = get(sprite, "gl-key");
    std::string key = toKey(gl_key);
    obj *o = toObj(map_view, qimages.at(key));
    QImage &img = o->qi_copy;
    saveImg(arch, img, key+".png");
  }

  cl_object animations = cl_funcall(2, toList, cl_funcall(2, get_animations, document.getValue())),
    anim_len = cl_funcall(2, len, animations);
  
  for (int i = 0; i < fixint(anim_len); i++) {
    cl_object animation_pair = cl_funcall(3, get_prop, animations, ecl_make_int32_t(i)),
      animation_id = cl_funcall(2, car, animation_pair),
      animation = cl_funcall(2, cdr, animation_pair),
      sprites = get(animation, "sprites"),
      sprites_len = cl_funcall(2, len, sprites);
    for(int sprite_i = 0; sprite_i < fixint(sprites_len); sprite_i++) {
      cl_object sprite = cl_funcall(3, get_prop, sprites, ecl_make_int32_t(sprite_i)),
  	sprite_id = cl_funcall(2, clean_key, cl_funcall(4, format, ECL_NIL, c_string_to_object("\"~a\""), get(sprite, "id"))),
  	gl_key = get(sprite, "gl-key");
      cl_funcall(4, format, ECL_T, c_string_to_object("\"saving animated subsprite ~a~%\""), sprite_id);
      std::string key = toKey(gl_key);
      obj *o = toObj(map_view, qimages.at(key));
      QImage &img = o->qi_copy;
      saveImg(arch, img, key+".png");
    }
  }

}

void editorController::saveTo(QString filename) {
    QFile f(filename);
  f.remove();
  ZipArchive arch(filename.toStdString());
  arch.open(ZipArchive::WRITE);
  puts("Trying root.toLISP();");
  cl_object prin1 = makefn("prin1-to-string");
  std::string rootLisp = ecl_string_to_string(cl_funcall(2, prin1, document.getValue()));

  puts("got root lisp");
  
  arch.addData("root.lisp", rootLisp.c_str(), rootLisp.size());
  dumpTextures(arch);
  arch.close();
  for(auto tmp: tempFiles) {
    tmp->close();
    delete tmp;
  }
  tempFiles.clear();
  printf("Saved root to %s\n", filename.toStdString().c_str());
}

void editorController::loadFrom(QString fname) {
  clearDrawQueues();
  
  ZipArchive arch(fname.toStdString());
  arch.open(ZipArchive::READ_ONLY);
  auto entries = arch.getEntries();
  int i = 0;
  for(ZipEntry entry: entries) {
    std::string name = entry.getName();
    if(name == "root.lisp") {
      cl_object load_root = makefn("qmapper.root:load-root");
      assert( i == 0);
      std::string data = entry.readAsText();
      data = ("\""+QString(data.c_str()).replace("\\\"", "\\\\\"").replace("\"", "\\\"")+"\"").toStdString();

      cl_object dada = c_string_to_object(data.c_str());
      document.setValue(cl_funcall(2, load_root, dada));
      continue;
    }

    QString qname(name.c_str());
    if(qname.endsWith(".png")) {
      printf("Reading in surface %s\n", qname.toStdString().c_str());
      auto dada = static_cast<const uchar*>(entry.readAsBinary());
      int len = entry.getSize();
      QImage img;
      img.loadFromData(dada, len);
      
      auto id = obj::make(img, qname.replace(".png", "").toStdString());
      qimages[id] = id;
      puts("READ IT");
    }
  }
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

