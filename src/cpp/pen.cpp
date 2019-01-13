#include <QMessageBox>
#include <pen.h>

void Pen::use(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  puts("Trying to pen");
  e->setTileAt(tilex, tiley);
  puts("Did pen");
}
