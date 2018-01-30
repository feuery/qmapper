#include <scroll.h>

Scroll::Scroll(QWidget *p): QScrollArea(p) {
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void Scroll::resizeEvent(QResizeEvent *e) {
  // Renderer *r = dynamic_cast<Renderer*>(viewport());
  // r->resizeEvent(e);
}

void Scroll::paintEvent(QPaintEvent *e) {
  // Renderer *r = dynamic_cast<Renderer*>(viewport());
  // r->paintEvent(e);
}

void Scroll::mouseMoveEvent(QMouseEvent *e) {
  // Renderer *r = dynamic_cast<Renderer*>(viewport());
  // r->mouseMoveEvent(e);
}
void Scroll::mousePressEvent(QMouseEvent *e) {
  // Renderer *r = dynamic_cast<Renderer*>(viewport());
  // r->mousePressEvent(e);
}
void Scroll::mouseReleaseEvent(QMouseEvent *e) {
  // Renderer *r = dynamic_cast<Renderer*>(viewport());
  // r->mouseReleaseEvent(e);
}
