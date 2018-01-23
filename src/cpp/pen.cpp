#include <pen.h>
#include <map.h>

void Pen::use(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  e->setTileAt(tilex, tiley);
}
