#ifndef ANIMATEDSPRITECONTAINER_H
#define ANIMATEDSPRITECONTAINER_H

#include <animatedSprite.h>
#include <spriteContainer.h>
#include <renderable.h>

class Animatedspritecontainer: public animatedsprite, public Renderable{
public:

  Animatedspritecontainer();
  ~Animatedspritecontainer();

  void setX (int newX);
  int getX ();
  void setY (int newY);
  int getY ();
  int maxFrames ();
  void advanceFrame ();
  void setAngle (float newangle);
  float getAngle ();

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
  virtual void render(Renderer *parent) override;
  virtual int getRenderId() override;

private:

};

#endif //ANIMATEDSPRITECONTAINER_H
