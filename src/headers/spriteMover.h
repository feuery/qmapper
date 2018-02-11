#ifndef SPRITEMOVER_H
#define SPRITEMOVER_H

#include <tool.h>
#include <animatedSprite.h>

class Spritemover: public Tool{
public:
  virtual void use(QMouseEvent *event, int tilex, int tiley, editorController *e) override;
  virtual void mouseDown(QMouseEvent *e, editorController *ec) override;
  virtual void mouseUp(QMouseEvent *e) override;
  virtual bool canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) override;
private:

  either<animatedsprite*, Sprite*> currentSprite;

};

#endif //SPRITEMOVER_H