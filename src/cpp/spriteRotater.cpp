#include <spriteRotater.h>

void Spriterotater::mouseDown(QMouseEvent *e, editorController *ec) {
  SCM findNearest = scm_c_lookup("Map-findNearest");
  SCM m = getChosenMap();
  currentSprite = scm_call_3(findNearest,
			     m,
			     scm_from_int(e->x()),
			     scm_from_int(e->y()));
  qDebug() << "Found the nearest sprite";
}

void Spriterotater::mouseUp(QMouseEvent *e) {
  currentSprite = SCM_BOOL_F;
  qDebug() << "Reset nearest sprite";
}

bool Spriterotater::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return scm_is_true(currentSprite);
}

float angle(int x1, int y1,
	    int x2, int y2) {
  int a = x2-x1,
    b = -(y2-y1);
  return atan2(a, b);
}

void Spriterotater::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {

  static SCM is_sprite_fn = scm_c_lookup("is-sprite?"),
    push_sprite_to_chosen_map = scm_c_lookup("push-sprite-to-chosen-map");
  bool is_sprite = scm_is_true(scm_call_1(is_sprite_fn, currentSprite));

  SCM getX = scm_c_lookup(is_sprite? "Sprite-x":
			  "animatedsprite-x"),
    getY = scm_c_lookup(is_sprite? "Sprite-y":
			"animatedsprite-y"),
    setAngle = scm_c_lookup(is_sprite? "set-Sprite-angle!":
			    "set-animatedsprite-angle!");
  float final_angle = angle(event->x(), event->y(),
		      scm_to_int(scm_call_1(getX, currentSprite)),
		      scm_to_int(scm_call_1(getY, currentSprite)));

  currentSprite = scm_call_2(setAngle, currentSprite, scm_from_double(final_angle));
  editorController::instance->document = scm_call_2(push_sprite_to_chosen_map,
						    editorController::instance->document,
						    currentSprite);
}
