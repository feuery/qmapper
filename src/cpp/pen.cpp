#include <pen.h>

void Pen::use(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  e->setTileAt(tilex, tiley);
}
