#ifndef PEN_H
#define PEN_H

#include <tool.h>

class Pen: public Tool{
public:
  virtual void use(QMouseEvent *event, int tilex, int tiley, editorController *e) override;
  virtual bool canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) override;

};

#endif //PEN_H
