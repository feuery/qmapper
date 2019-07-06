#ifndef HIT_TOOL
#define HIT_TOOL

#include <tool.h>

class Hit_tool: public Tool{
public:
  virtual void use(QMouseEvent *event, int tilex, int tiley, editorController *e) override;
  virtual bool isHitTool() override;
  virtual bool canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) override;
};


#endif
