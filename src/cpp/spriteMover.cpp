#include <spriteMover.h>

void Spritemover::mouseDown(QMouseEvent *e, editorController *ec) {
}

void Spritemover::mouseUp(QMouseEvent *e) {
}

bool Spritemover::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return true;
}

void Spritemover::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  puts("using spritemover");
  cl_object is_sprite_fn = makefn("qmapper.sprite:is-sprite?"),
    format = makefn("format"),
    m = getChosenMap(),
    findNearest = makefn("qmapper.map:map-findnearest");
  cl_funcall(4, format, ECL_T, c_string_to_object("\"m is ~a~%\""), m);
  cl_object currentSprite = cl_funcall(4, findNearest,
				       m,
				       ecl_make_int32_t(event->x()),
				       ecl_make_int32_t(event->y()));
  
  bool is_sprite = cl_funcall(2, is_sprite_fn, currentSprite) == ECL_T;

  puts(is_sprite ? "moving sprite": "moving animation");

  cl_object setX = makefn(is_sprite? "qmapper.sprite:set-Sprite-x!": "qmapper.animatedsprite:set-animatedsprite-x!"),
    setY = makefn(is_sprite? "qmapper.sprite:set-Sprite-y!": "qmapper.animatedsprite:set-animatedsprite-y!"),
    push_sprite = makefn("qmapper.root:push-sprite2"),
    push_anim = makefn("qmapper.root:push-animation");

  cl_funcall(4, format, ECL_T, c_string_to_object("\"currentSprite is ~a~%\""), currentSprite);
  

  currentSprite = cl_funcall(3, setX, currentSprite, ecl_make_int32_t(event->x()));
  currentSprite = cl_funcall(3, setY, currentSprite, ecl_make_int32_t(event->y()));

  if(is_sprite) {
    editorController::instance->document.setValue(cl_funcall(5, push_sprite,
							     editorController::instance->document.getValue(),
							     get(m, "id"),
							     get(currentSprite, "id"),
							     currentSprite));
  }
  else {
    editorController::instance->document.setValue(cl_funcall(4, push_anim,
							     editorController::instance->document.getValue(),
							     get(m, "id"),
							     currentSprite));
  }
  
  puts("used spritemover");
}
