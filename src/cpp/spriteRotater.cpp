#include <spriteRotater.h>
#include <map.h>

void Spriterotater::mouseDown(QMouseEvent *e, editorController *ec) {
  Map *m = toMap(editorController::instance->document.fetchRegister("Map",
								    editorController::instance->indexOfChosenMap));
  currentSprite = m->findNearest(e->x(), e->y());
  qDebug() << "Found the nearest sprite";
}

void Spriterotater::mouseUp(QMouseEvent *e) {
  currentSprite = either<animatedsprite*, Sprite*>();
  qDebug() << "Reset nearest sprite";
}

bool Spriterotater::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return currentSprite != either<animatedsprite*, Sprite*>();
}

float angle(int x1, int y1,
	    int x2, int y2) {
  int a = x2-x1,
    b = -(y2-y1);
  return // b == 0? 0.0f:
    atan2(a, b);
}

void Spriterotater::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  // Eti kulma ja aseta se radiaaneina
  if(currentSprite.a) {
    currentSprite.a->setAngle(angle(event->x(), event->y(),
				    currentSprite.a->getX(), currentSprite.a->getY()));
  }
  else if(currentSprite.b) {
        currentSprite.b->setAngle(angle(event->x(), event->y(),
				    currentSprite.b->getX(), currentSprite.b->getY()));
  }
}
