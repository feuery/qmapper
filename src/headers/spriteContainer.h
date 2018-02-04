#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <sprite.h>

class Spritecontainer: public Sprite, public obj {
public:

  static Spritecontainer* make(Renderer *parent, const char *text_path);

  void setX (int newX) override;
  int getX () override;
  void setY (int newY) override;
  int getY () override;
  void setAngle (float newangle) override;
  float getAngle () override;

private:
  Spritecontainer(Renderer *parent, const char *text_path);
};

#endif //SPRITECONTAINER_H
