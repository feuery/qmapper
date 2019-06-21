#ifndef SELECTION_H
#define SELECTION_H

#include <tool.h>

class SelectionTool: public Tool {
public:
  virtual void use(QMouseEvent *event, int tilex, int tiley, editorController *e) override;
  virtual void mouseDown(QMouseEvent *e, editorController *ec) override;
  virtual void mouseUp(QMouseEvent *e) override;
};

#endif
