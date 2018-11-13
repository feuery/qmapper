#include <QMessageBox>
#include <pen.h>

void Pen::use(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  QMessageBox::information(nullptr, QString("Lolmoi: x: %1, x: %2").arg(tilex).arg(tiley), QString("Lolmoi: x: %1, x: %2").arg(tilex).arg(tiley));
  // e->setTileAt(tilex, tiley);
}
