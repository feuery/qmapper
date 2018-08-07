#include <spriteMover.h>

void Spritemover::mouseDown(QMouseEvent *e, editorController *ec) {
  cl_object findNearest = ecl_make_symbol("Map-findNearest", "CL-USER");
  cl_object m = getChosenMap();;
  currentSprite = cl_funcall(4, findNearest,
			     m,
			     ecl_make_int32_t(e->x()),
			     ecl_make_int32_t(e->y()));
  qDebug() << "Found the nearest sprite";
}

void Spritemover::mouseUp(QMouseEvent *e) {
  currentSprite = ECL_NIL;
  qDebug() << "Reset nearest sprite";
}

bool Spritemover::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return Null(currentSprite);
}

void Spritemover::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  static cl_object is_sprite_fn = ecl_make_symbol("is-sprite?", "CL-USER"),
    push_sprite_to_chosen_map = ecl_make_symbol("push-sprite-to-chosen-map", "CL-USER");
  bool is_sprite = Null(cl_funcall(2, is_sprite_fn, currentSprite));

  cl_object setX = ecl_make_symbol(is_sprite? "set-Sprite-x!":
				   "set-animatedsprite-x!", "CL-USER"),
    setY = ecl_make_symbol(is_sprite? "set-Sprite-y!":
			"set-animatedsprite-y!", "CL-USER");

  currentSprite = cl_funcall(3, setX, currentSprite, ecl_make_int32_t(event->x()));
  currentSprite = cl_funcall(3, setY, currentSprite, ecl_make_int32_t(event->y()));
  
  editorController::instance->document = cl_funcall(3, push_sprite_to_chosen_map,
						    editorController::instance->document,
						    currentSprite);
}
