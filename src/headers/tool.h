#ifndef TOOL_H
#define TOOL_H

#include <QMouseEvent>
#include <editorController.h>

class Tool{
public:
  virtual void use(QMouseEvent *event, int tilex, int tiley, editorController *e) = 0;
  virtual bool canUse(QMouseEvent *event, int tilex, int tiley, editorController *e);

  virtual void mouseDown(QMouseEvent *e, editorController *ec);
  virtual void mouseUp(QMouseEvent *e);
  bool toolAppliedTo(int tilex, int tileY);
  void registerDrag(int tilex, int tiley);

  void doUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
    if(canUse(event, tilex, tiley, e)) use(event, tilex, tiley, e);
  }

private:
  std::vector<std::vector<bool>> mouse_map;
};
#else

class Tool;

#endif //TOOL_H
