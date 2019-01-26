#include <spriteRotater.h>

void Spriterotater::mouseDown(QMouseEvent *e, editorController *ec) {
}

void Spriterotater::mouseUp(QMouseEvent *e) {
}

bool Spriterotater::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return true;
}

float angle(int x1, int y1,
	    int x2, int y2) {
  int a = x2-x1,
    b = -(y2-y1);
  return atan2(a, b);
}

void Spriterotater::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  cl_object is_sprite_fn = makefn("qmapper.sprite:is-sprite?"),
    push_sprite = makefn("qmapper.root:push-sprite2"),
    m = getChosenMap(),
    findNearest = makefn("qmapper.map:map-findnearest"),    
    currentSprite = cl_funcall(4, findNearest,
			       m,
			       ecl_make_int32_t(event->x()),
			       ecl_make_int32_t(event->y())),
    getX = makefn("qmapper.sprite:Sprite-x"),
    getY = makefn("qmapper.sprite:Sprite-y"),
    setAngle = makefn("qmapper.sprite:set-Sprite-angle!");
  float final_angle = angle(event->x(), event->y(),
			    fixint(cl_funcall(2, getX, currentSprite)),
			    fixint(cl_funcall(2, getY, currentSprite)));

  currentSprite = cl_funcall(3, setAngle, currentSprite, ecl_make_double_float(final_angle));
  editorController::instance->document.setValue(cl_funcall(5, push_sprite,
							   editorController::instance->document.getValue(),
							   get(m, "id"),
							   get(currentSprite, "id"),
							   currentSprite));
}
