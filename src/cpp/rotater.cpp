#include <rotater.h>

void Rotater::use(QMouseEvent *event, int tilex, int tiley, editorController *e){
  if(!toolAppliedTo(tilex, tiley)) {
    // TODO build the state to see angle between tile on which mouse btn was set down and mouse's current position AND set that angle as tile's angle.
    e->rotateTile90Deg(tilex, tiley);
  }
}
