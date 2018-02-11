#include <spriteMover.h>
#include <map.h>

void Spritemover::mouseDown(QMouseEvent *e, editorController *ec) {
  Map *m = toMap(editorController::instance->document.fetchRegister("Map",
								    editorController::instance->indexOfChosenMap));
  currentSprite = m->findNearest(e->x(), e->y());
  qDebug() << "Found the nearest sprite";
}

void Spritemover::mouseUp(QMouseEvent *e) {
  currentSprite = either<animatedsprite*, Sprite*>();
  qDebug() << "Reset nearest sprite";
}

bool Spritemover::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return currentSprite != either<animatedsprite*, Sprite*>();
}

void Spritemover::use(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  if(currentSprite.a) {
    currentSprite.a->setX(event->x());
    currentSprite.a->setY(event->y());
    qDebug()<<"Set .a to " << event->x() << ", " << event->y();
  }
  else if(currentSprite.b) {
    currentSprite.b->setX(event->x());
    currentSprite.b->setY(event->y());
    qDebug()<<"Set .b to " << event->x() << ", " << event->y();
  }
}
