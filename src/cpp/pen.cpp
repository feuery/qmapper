#include <pen.h>
#include <map.h>

bool Pen::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  Map *m = toMap(e->document.fetchRegister("Map", e->indexOfChosenMap));
  return tilex < m->width() && tiley < m->height() && tilex >= 0 && tiley >= 0;
}

void Pen::use(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  e->setTileAt(tilex, tiley);
}
