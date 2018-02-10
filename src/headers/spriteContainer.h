#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <sprite.h>

class Spritecontainer: public Sprite, public Renderable {
public:
  int renderId;
  static Spritecontainer* make(Renderer *parent, const char *text_path);

  ~Spritecontainer();

  void setX (int newX) override;
  int getX () override;
  void setY (int newY) override;
  int getY () override;
  void setAngle (float newangle) override;
  float getAngle () override;

  void render(QOpenGLFunctions_4_3_Core *f) override;
  void render(Renderer *parent) override;
  int getRenderId() override;

private:
  Spritecontainer(Renderer *parent, const char *text_path);

  Renderer *parent;
  obj* getObject();
};

#endif //SPRITECONTAINER_H
