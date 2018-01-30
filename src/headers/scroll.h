#ifndef SCROLL_H
#define SCROLL_H

#include <QScrollArea>
#include <renderer.h>

class Scroll: public // Abstract
QScrollArea {
public:
  Scroll(QWidget *p);

protected:
  void resizeEvent(QResizeEvent *e) override;
  void paintEvent(QPaintEvent *e)  override;
  void mouseMoveEvent(QMouseEvent *e)  override;
  void mousePressEvent(QMouseEvent *e)  override;
  void mouseReleaseEvent(QMouseEvent *e) override;
private:

};
#else
class Scroll;
#endif //SCROLL_H
