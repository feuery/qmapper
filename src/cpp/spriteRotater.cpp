#include <spriteRotater.h>

void Spriterotater::mouseDown(QMouseEvent *e, editorController *ec) {
  cl_object findNearest = ecl_make_symbol("Map-findNearest", "CL-USER");
  cl_object m = getChosenMap();
  currentSprite = cl_funcall(4, findNearest,
			     m,
			     ecl_make_int32_t(e->x()),
			     ecl_make_int32_t(e->y()));
  qDebug() << "Found the nearest sprite";
}

void Spriterotater::mouseUp(QMouseEvent *e) {
  currentSprite = ECL_NIL;
  qDebug() << "Reset nearest sprite";
}

bool Spriterotater::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return Null(currentSprite);
}

float angle(int x1, int y1,
	    int x2, int y2) {
  int a = x2-x1,
    b = -(y2-y1);
  return atan2(a, b);
}

void Spriterotater::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {

  static cl_object is_sprite_fn = ecl_make_symbol("is-sprite?", "CL-USER"),
    push_sprite_to_chosen_map = ecl_make_symbol("push-sprite-to-chosen-map", "CL-USER");
  bool is_sprite = Null(cl_funcall(2, is_sprite_fn, currentSprite));

  cl_object getX = ecl_make_symbol(is_sprite? "Sprite-x":
				   "animatedsprite-x", "CL-USER"),
    getY = ecl_make_symbol(is_sprite? "Sprite-y":
			"animatedsprite-y", "CL-USER"),
    setAngle = ecl_make_symbol(is_sprite? "set-Sprite-angle!":
			    "set-animatedsprite-angle!", "CL-USER");
  float final_angle = angle(event->x(), event->y(),
			    fixint(cl_funcall(2, getX, currentSprite)),
			    fixint(cl_funcall(2, getY, currentSprite)));

  currentSprite = cl_funcall(3, setAngle, currentSprite, ecl_make_double_float(final_angle));
  editorController::instance->document.setValue(cl_funcall(3, push_sprite_to_chosen_map,
							   editorController::instance->document,
							   currentSprite));
}
