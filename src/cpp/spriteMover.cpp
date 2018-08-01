#include <spriteMover.h>

void Spritemover::mouseDown(QMouseEvent *e, editorController *ec) {
  SCM findNearest = scm_c_lookup("Map-findNearest");
  SCM m = getChosenMap();;
  currentSprite = scm_call_3(findNearest,
			     m,
			     scm_from_int(e->x()),
			     scm_from_int(e->y()));
  qDebug() << "Found the nearest sprite";
}

void Spritemover::mouseUp(QMouseEvent *e) {
  currentSprite = SCM_BOOL_F;
  qDebug() << "Reset nearest sprite";
}

bool Spritemover::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return scm_is_true(currentSprite);
}

void Spritemover::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  static SCM is_sprite_fn = scm_c_lookup("is-sprite?"),
    push_sprite_to_chosen_map = scm_c_lookup("push-sprite-to-chosen-map");
  bool is_sprite = scm_is_true(scm_call_1(is_sprite_fn, currentSprite));

  SCM setX = scm_c_lookup(is_sprite? "set-Sprite-x!":
			  "set-animatedsprite-x!"),
    setY = scm_c_lookup(is_sprite? "set-Sprite-y!":
			"set-animatedsprite-y!");

  currentSprite = scm_call_2(setX, currentSprite, scm_from_int(event->x()));
  currentSprite = scm_call_2(setY, currentSprite, scm_from_int(event->y()));
  
  editorController::instance->document = scm_call_2(push_sprite_to_chosen_map,
						    editorController::instance->document,
						    currentSprite);
}
