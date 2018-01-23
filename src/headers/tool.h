#ifndef TOOL_H
#define TOOL_H

#include <QMouseEvent>
#include <editorController.h>

class Tool{
public:
  virtual void use(QMouseEvent *event, int tilex, int tiley, editorController *e) = 0;
  virtual bool canUse(QMouseEvent *event, int tilex, int tiley, editorController *e);

  void doUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
    if(canUse(event, tilex, tiley, e)) use(event, tilex, tiley, e);
  }
};
#else

class Tool;

#endif //TOOL_H
